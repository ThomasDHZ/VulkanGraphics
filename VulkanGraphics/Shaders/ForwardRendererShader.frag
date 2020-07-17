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

struct TextureFlags
{
    int DiffuseMapFlag;
    int SpecularMapFlag;
    int NormalMapFlag;
    int DisplacementMapFlag;
    int AlphaMapFlag;
    int CubeMapFlag;
};

struct Material
{
	vec3 Diffuse;
	vec3 Specular;
	float Shininess;
    float Alpha;
    float Reflection;
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
layout(binding = 6) uniform sampler2D EmissionMap;
layout(binding = 7) uniform samplerCube CubeMap;
layout(binding = 8) uniform MeshProperties
{
    TextureFlags MapFlags;
    Material material;
    vec2 SpriteUV;
    float height;
    int FlipTexture;
} meshProperties;
layout(binding = 9) uniform LightProperties
{
    DirectionalLight directionalLight;
    PointLight pointLight;
    SpotLight spotLight;
    vec3 viewPos;
} lightProperties;


vec2 AnimationCoords = vec2(TexCoords.x + meshProperties.SpriteUV.x, TexCoords.y + meshProperties.SpriteUV.y);

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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightDir = normalize(-light.Direction);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), meshProperties.material.Shininess);

    if(meshProperties.MapFlags.DiffuseMapFlag == 1)
    {
        ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
        diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    }
    else
    {
        ambient = light.Ambient * meshProperties.material.Diffuse;
        diffuse = light.Diffuse * diff * meshProperties.material.Diffuse;
    }
    if(meshProperties.MapFlags.SpecularMapFlag == 1)
    {
        specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    }
    else
    {
        specular = light.Specular * spec * meshProperties.material.Specular;
    }
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), meshProperties.material.Shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    

    if(meshProperties.MapFlags.DiffuseMapFlag == 1)
    {
        ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
        diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    }
    else
    {
        ambient = light.Ambient * meshProperties.material.Diffuse;
        diffuse = light.Diffuse * diff * meshProperties.material.Diffuse;
    }
    if(meshProperties.MapFlags.SpecularMapFlag == 1)
    {
        specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    }
    else
    {
        specular = light.Specular * spec * meshProperties.material.Specular;
    }
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightDir = normalize(light.Position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), meshProperties.material.Shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.Direction)); 
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);

    if(meshProperties.MapFlags.DiffuseMapFlag == 1)
    {
        ambient = light.Ambient * vec3(texture(DiffuseMap, AnimationCoords));
        diffuse = light.Diffuse * diff * vec3(texture(DiffuseMap, AnimationCoords));
    }
    else
    {
        ambient = light.Ambient * meshProperties.material.Diffuse;
        diffuse = light.Diffuse * diff * meshProperties.material.Diffuse;
    }
    if(meshProperties.MapFlags.SpecularMapFlag == 1)
    {
        specular = light.Specular * spec * vec3(texture(SpecularMap, AnimationCoords));
    }
    else
    {
        specular = light.Specular * spec * meshProperties.material.Specular;
    }
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 CalcReflection(vec3 InputPixel)
{
    vec3 I = normalize(FragPos - lightProperties.viewPos);
    vec3 R = reflect(I, normalize(Normal));
    return InputPixel + (texture(CubeMap, R).rgb * meshProperties.material.Reflection);
}

void main()
{
    if(meshProperties.FlipTexture == 1)
    {
        vec2 Flipped = vec2(-TexCoords.x, TexCoords.y);
        AnimationCoords = vec2(Flipped.x + meshProperties.SpriteUV.x, Flipped.y + meshProperties.SpriteUV.y);
    }
    else
    {
        AnimationCoords = vec2(TexCoords.x + meshProperties.SpriteUV.x, TexCoords.y + meshProperties.SpriteUV.y);
    }

    RemoveAlphaPixels();

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(lightProperties.viewPos - FragPos);

    vec3 result = CalcDirLight(lightProperties.directionalLight, norm, viewDir);
    result += CalcPointLight(lightProperties.pointLight, norm, FragPos, viewDir);
    result += CalcSpotLight(lightProperties.spotLight, norm, FragPos, viewDir);
   // result += CalcReflection(result);
    
    outColor = vec4(result, 1.0);
} 
