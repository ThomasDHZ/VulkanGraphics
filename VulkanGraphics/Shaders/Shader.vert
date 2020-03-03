  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexureCoord;
layout(location = 3) in vec3 Tangant;
layout(location = 4) in vec2 BiTangant;

layout(location = 0) out vec3 fragPosition;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec2 fragTexCoord;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(Position, 1.0);
    fragPosition = vec3(ubo.model * vec4(Position, 1.0f));
	fragNormal = Normal;
    fragTexCoord = TexureCoord;
}

