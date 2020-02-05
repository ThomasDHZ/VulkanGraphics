  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D DiffuseMap;
layout(binding = 2) uniform sampler2D SpecularMap;
layout(binding = 3) uniform samplerCube skybox;
layout(binding = 4) uniform FragmentUniformBufferObject 
{
    vec3 cameraPos;
} ubo;

layout(location = 0) in vec3 fragPosition;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
//	vec3 I = normalize(fragPosition - ubo.cameraPos);
//    vec3 R = reflect(I, normalize(fragNormal));
//    outColor = vec4(texture(skybox, R).rgb, 1.0);
outColor = texture(DiffuseMap, fragTexCoord);
}
