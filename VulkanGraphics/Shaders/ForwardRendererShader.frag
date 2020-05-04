#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform AmbiantLight
{
	vec3 Color;
	float Strength;
} ambiantLight;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
	vec3 AmbientColor = ambiantLight.Strength * ambiantLight.Color;
    outColor = vec4(AmbientColor * texture(DiffuseMap, fragTexCoord).rgb, 1.0f);
}
