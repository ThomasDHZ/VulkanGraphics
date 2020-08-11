#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D texSampler;
layout(binding = 1) uniform FrameBufferSettings
{
    float Gamma;
    float HDRValue;
} settings;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main() {
    vec3 hdrColor = texture(texSampler, fragTexCoord).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * settings.HDRValue);
    vec3 color = pow(result, vec3(1.0/settings.Gamma));
    outColor = vec4(color, 1.0f);
}