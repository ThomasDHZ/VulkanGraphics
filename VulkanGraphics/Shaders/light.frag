  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform LightingStruct 
{
	vec3 viewPos;
	float shininess;

	vec3 Position;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
} Light;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

layout(location = 0) out vec4 FragColor;

void main() 
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
