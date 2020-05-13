#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform Material
{
	vec3 Diffuse;
	vec3 Specular;
	vec3 Shininess;
} material;
layout(binding = 4) uniform AmbiantLight
{
	vec3 Color;
	float Strength;
} ambiantLight;
layout(binding = 5) uniform Lighter
{
	vec3 lightPos;
	vec3 viewPos;
	vec3 lightColor;
	vec3 objectColor;
} light;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UVs;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = texture(DiffuseMap, UVs);
}
