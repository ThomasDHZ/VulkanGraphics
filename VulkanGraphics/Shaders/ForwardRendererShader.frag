#version 450
#extension GL_ARB_separate_shader_objects : enable

struct DirectionalLight {
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};

struct PointLight {
    vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

	float Constant;
    float Linear;
    float Quadratic;
};

struct SpotLight 
{
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
	
	float CutOff;
    float OuterCutOff;
    float Constant;
    float Linear;
    float Quadratic;
};

struct Material
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
};

#define NR_POINT_LIGHTS 4

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 TangentLightPos;
layout(location = 3) in vec3 TangentLightPos2;
layout(location = 4) in vec3 TangentViewPos;
layout(location = 5) in vec3 TangentFragPos;
  
layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform sampler2D NormalMap;
layout(binding = 4) uniform sampler2D DisplacementMap;
layout(binding = 5) uniform sampler2D AlphaMap;
layout(binding = 6) uniform samplerCube CubeMap;
layout(binding = 7) uniform MeshProp
{
    DirectionalLight directionalLight;
    PointLight pointLight;
    SpotLight spotLight;
    Material material;
    vec3 viewPos;
    vec2 SpriteUV;
} mesh;


vec2 AnimationCoords = vec2(TexCoords.x + mesh.SpriteUV.x, TexCoords.y + mesh.SpriteUV.y);

void RemoveAlphaPixels()
{
    if((textureSize(AlphaMap, 0).x > 1 &&
        texture(AlphaMap, AnimationCoords).r == 0) ||
        texture(DiffuseMap, AnimationCoords).a == 0)
   {
        discard;
   }
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.Direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mesh.material.Shininess);

    vec3 ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mesh.material.Shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    

    vec3 ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mesh.material.Shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

//vec3 CalcReflection(vec3 InputPixel)
//{
   // vec3 I = normalize(FragPos - mesh.viewPos);
    //vec3 R = reflect(I, normalize(Normal));
  //  return mix(InputPixel, texture(CubeMap, R).rgb, 1.0f);
//}


vec3 Lighterz(vec3 normal, vec3 fragPos, vec3 viewDir, int x)
{
    vec3 color = texture(DiffuseMap, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;

    // diffuse
    vec3 lightDir; 
    if(x == 0)
    {
        lightDir = normalize(TangentLightPos - TangentFragPos);
    }
    else
    {
        lightDir = normalize(TangentLightPos2 - TangentFragPos);
    }

    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.2) * spec;
    return ambient + diffuse + specular;
}

void main()
{
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = TexCoords;

    vec3 normal = texture(NormalMap, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
   
    
   vec3 result = Lighterz(normal, FragPos, viewDir, 0);
   result += Lighterz(normal, FragPos, viewDir, 1);

    outColor = vec4(result,  1.0);

    //RemoveAlphaPixels();

   // vec3 norm = normalize(Normal);
   // vec3 viewDir = normalize(mesh.viewPos - FragPos);

   // vec3 result = CalcDirLight(mesh.directionalLight, norm, viewDir);
   // result += CalcPointLight(mesh.pointLight, norm, FragPos, viewDir);
   // result += CalcSpotLight(mesh.spotLight, norm, FragPos, viewDir);
   // result += CalcReflection(result);
   // outColor = vec4(result, 1.0f);
} 
