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
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;
  
layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform sampler2D NormalMap;
layout(binding = 4) uniform sampler2D DisplacementMap;
layout(binding = 5) uniform sampler2D AlphaMap;
layout(binding = 6) uniform samplerCube CubeMap;
layout(binding = 7) uniform MeshProp
{
	SpotLight spotLight;
    DirectionalLight directionalLight;
	PointLight pointLight[NR_POINT_LIGHTS];
	Material material;
    vec3 viewPos;
    vec3 SpriteUV;
    float timer;
} mesh;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
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

vec3 CalcReflection(vec3 InputPixel)
{
    vec3 I = normalize(FragPos - mesh.viewPos);
    vec3 R = reflect(I, normalize(Normal));
    return InputPixel + (texture(CubeMap, R).rgb * 0.5f);
}

void main()
{
    RemoveAlphaPixels();

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(mesh.viewPos - FragPos);

    vec3 result = CalcDirLight(mesh.directionalLight, norm, viewDir);

    for(int i = 0; i < 4; i++)
        result += CalcPointLight(mesh.pointLight[i], norm, FragPos, viewDir);    

    result += CalcSpotLight(mesh.spotLight, norm, FragPos, viewDir);    
    result = CalcReflection(result);
    outColor = vec4(result, 1.0f);
} 

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-mesh.directionalLight.Direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mesh.material.Shininess);

    vec3 ambient = mesh.directionalLight.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 diffuse = mesh.directionalLight.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 specular = mesh.directionalLight.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mesh.material.Shininess);

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
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mesh.material.Shininess);
    // attenuation
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = mesh.directionalLight.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 diffuse = mesh.directionalLight.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    vec3 specular = mesh.directionalLight.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}