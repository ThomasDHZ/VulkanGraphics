#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DepthMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float LinearizeDepth(float depth)
{
    float near_plane = 0.1; 
    float far_plane  = 100.0;

    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main() 
{
    float depth = texture(DepthMap, fragTexCoord).r;
	outColor = vec4(vec3(1.0-LinearizeDepth(depth)), 1.0);
}