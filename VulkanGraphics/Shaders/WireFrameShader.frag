#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D diffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D depthMap;
layout(binding = 5) uniform sampler2D AlphaMap;
layout(binding = 6) uniform sampler2D EmissionMap;
layout(binding = 7) uniform MeshProperties
{
    int dummydata;
} meshProperties;
layout(binding = 8) uniform Light
{
    int dummydata;
} light;

layout(location = 0) out vec4 FragColor;

void main()
{           
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0);
}