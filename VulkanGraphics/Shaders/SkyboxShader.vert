  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 aPos;

layout(location = 0) out vec3 TexCoords;

void main() {
	TexCoords = aPos;
	vec4 pos = ubo.projection * ubo.view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

