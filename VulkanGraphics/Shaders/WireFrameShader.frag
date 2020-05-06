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

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
