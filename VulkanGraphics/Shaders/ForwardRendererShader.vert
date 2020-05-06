#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec3 MeshPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 UVs;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition, 1.0);

	MeshPos = vec3(ubo.model * vec4(inPosition, 1.0));
    Normal = mat3(transpose(inverse(ubo.model))) * inNormal;  
    UVs = inTexCoord;
}

