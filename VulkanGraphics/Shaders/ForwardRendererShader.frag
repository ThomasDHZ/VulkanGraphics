#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform AmbiantLight
{
	vec3 Color;
	float Strength;
} ambiantLight;
layout(binding = 4) uniform Lighter
{
	vec3 Position;
	vec3 Color;
	vec3 CameraPosition;
} light;

layout(location = 0) in vec3 MeshPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UVs;

layout(location = 0) out vec4 outColor;

void main() 
{
	vec3 AmbientColor = ambiantLight.Strength * ambiantLight.Color;

	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.Position - MeshPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 DiffuseColor = diff * light.Color;
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(light.CameraPosition - MeshPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 SpecularColor = specularStrength * spec * light.Color;  

	outColor = vec4((AmbientColor + DiffuseColor + SpecularColor) * texture(DiffuseMap, UVs).rgb, 1.0f);
}
