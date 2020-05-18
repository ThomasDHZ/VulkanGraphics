#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;
layout(location = 3) in vec3 Tangent;
layout(location = 4) in vec3 Bitangent;

layout(binding = 1) uniform Lighter
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

void main()
{
    outColor = vec4(light.ambient, 1.0);
} 


