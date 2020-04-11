#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform sampler2D BackgroundLayer;
layout(binding = 1) uniform sampler2D SpriteLayer;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() 
{
	vec4 FinalLayer = texture(BackgroundLayer, fragTexCoord);
	if(texture(SpriteLayer, fragTexCoord).a > 0.1f)
	{
		FinalLayer = texture(SpriteLayer, fragTexCoord);
	}
    outColor = FinalLayer;
}
