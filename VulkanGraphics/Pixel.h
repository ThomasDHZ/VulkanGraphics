#pragma once
typedef unsigned char byte;

struct Pixel
{
	byte Red;
	byte Green;
	byte Blue;
	byte Alpha;

	Pixel()
	{
		Red = 0xFF;
		Green = 0xFF;
		Blue = 0xFF;
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