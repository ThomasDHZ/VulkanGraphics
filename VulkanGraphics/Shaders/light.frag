  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D texSampler2;
layout(binding = 3) uniform LightingStruct 
{
	vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
} Lighting;

layout(location = 0) in vec3 fragNormal;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
    outColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
