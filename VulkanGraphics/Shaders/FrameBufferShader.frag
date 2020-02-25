#version 450

layout (input_attachment_index = 0, binding = 0) uniform subpassInput inputPosition;
layout (input_attachment_index = 1, binding = 1) uniform subpassInput inputNormal;
layout (input_attachment_index = 2, binding = 2) uniform subpassInput inputAlbedo;
layout (input_attachment_index = 3, binding = 3) uniform subpassInput inputDepth;

layout (location = 0) out vec4 outColor;

void main() 
{
		vec3 color = subpassLoad(inputPosition).rgb;
		color += subpassLoad(inputNormal).rgb;
		color += subpassLoad(inputAlbedo).rgb;
		outColor.rgb = color;

//		float depth = subpassLoad(inputDepth).r; 
// 		outColor.rgb = vec3(depth); 

}