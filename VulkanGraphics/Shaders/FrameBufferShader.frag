#version 450
layout(binding = 0) uniform sampler2D FrameBufferSampler;
layout(location = 0) in vec2 fragTexCoord;
layout(location = 0) out vec4 outColor;

void main() 
{
    vec3 color = texture(FrameBufferSampler, fragTexCoord).rgb;
    color = pow(color, vec3(1.0/2.2));
	outColor = vec4(color, 1.0f);
}