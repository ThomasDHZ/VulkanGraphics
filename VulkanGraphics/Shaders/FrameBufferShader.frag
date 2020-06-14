#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputColor;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputDepth;
layout (binding = 2) uniform FrameBufferSettings 
{
	bool HDR;
} ubo;

layout(location = 0) out vec4 outColor;

void main() 
{
	vec3 color = subpassLoad(inputColor).rgb;
	//color = vec3(1.0) - exp(-color * 1.0f);
	//color = pow(color, vec3(1.0/2.2));
	outColor.rgb = color;
}