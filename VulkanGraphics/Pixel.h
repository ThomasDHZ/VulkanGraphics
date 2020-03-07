#pragma once
typedef unsigned char byte;

struct Pixel
{
	byte Red = 0x0;
	byte Green = 0x0;
	byte Blue = 0x0;
	byte Alpha = 0xFF;

	Pixel()
	{
		Red = 0x0;
		Green = 0x0;
		Blue = 0x0;
		Alpha = 0xFF;
	}

	Pixel(byte red, byte green, byte blue)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = 0xFF;
	}

	Pixel(byte red, byte green, byte blue, byte alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}

	Pixel(glm::vec3 Color)
	{
		Red = Color.r;
		Green = Color.g;
		Blue = Color.b;
		Alpha = 0xFF;
	}

	Pixel(glm::vec4 Color)
	{
		Red = Color.r;
		Green = Color.g;
		Blue = Color.b;
		Alpha = Color.a;
	}
};