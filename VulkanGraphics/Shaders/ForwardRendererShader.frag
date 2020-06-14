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

struct Lightingz
{
    vec3 Position;
    vec3 Color;
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
    Lightingz lightz[4];
    vec3 viewPos;
} mesh;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec2 AnimationCoords = vec2(TexCoords.x + mesh.SpriteUV.x, TexCoords.y + mesh.SpriteUV.y);

void RemoveAlphaPixels()
{
    //if((textureSize(AlphaMap, 0).x > 1 &&
    //   texture(AlphaMap, AnimationCoords).r == 0) ||
    //   texture(DiffuseMap, AnimationCoords).a == 0)
   // {
   //     discard;
   // }
}

vec3 CalcReflection(vec3 InputPixel)
{
    vec3 I = normalize(FragPos - mesh.viewPos);
    vec3 R = reflect(I, normalize(Normal));
    return InputPixel + (texture(CubeMap, R).rgb * 0.5f);
}

void main()
{
    vec3 color = texture(DiffuseMap, TexCoords).rgb;
    vec3 normal = normalize(Normal);
    // ambient
    vec3 ambient = 0.0 * color;
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 4; i++)
    {
        // diffuse
        vec3 lightDir = normalize(mesh.lightz[i].Position - FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = mesh.lightz[i].Color * diff * color;      
        vec3 result = diffuse;        
        // attenuation (use quadratic as we have gamma correction)
        float distance = length(FragPos - mesh.lightz[i].Position);
        result *= 1.0 / (distance * distance);
        lighting += result;
                
    }
    outColor = vec4(ambient + lighting, 1.0);
} 
