#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec3 inBitangent;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec2 TexCoords;
layout(location = 3) out mat3 TBN;

mat3 TBNMatrix()
{
    mat3 NormalMatrix = transpose(inverse(mat3(ubo.model)));
    vec3 T = normalize(NormalMatrix * inTangent);
    vec3 N = normalize(NormalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    return TBN = transpose(mat3(T,B,N));
}

void main() 
{
    FragPos = vec3(ubo.model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(ubo.model))) * aNormal;  
    TexCoords = aTexCoords;
    TBN = TBNMatrix();
    gl_Position = ubo.proj * ubo.view * vec4(FragPos, 1.0);
}