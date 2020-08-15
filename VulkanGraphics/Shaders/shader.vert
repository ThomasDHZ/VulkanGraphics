#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    vec3 lightPos;
    vec3 viewPos;
} ubo;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout(location = 0) out vec3 FragPos;
layout(location = 1) out vec2 TexCoords;
layout(location = 2) out vec3 TangentLightPos;
layout(location = 3) out vec3 TangentViewPos;
layout(location = 4) out vec3 TangentFragPos;

void main() 
{
    FragPos = vec3(ubo.model * vec4(aPos, 1.0));   
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(ubo.model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));    
    TangentLightPos = TBN * ubo.lightPos;
    TangentViewPos  = TBN * ubo.viewPos;
    TangentFragPos  = TBN * FragPos;
        
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(aPos, 1.0);
}

