#include "Image.h"
#include <stb_image.h>
#include <stdexcept>

Image::Image()
{
}

Image::Image(std::string TexturePath)
{
	int texChannels;
	stbi_uc* pixels = stbi_load(TexturePath.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	ImageSize = Width * Height * 4;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	for (int x = 0; x <= ImageSize; x += 4)
	{
		ImageData.emplace_back(Pixel
			{
				pixels[x],
				pixels[x + 1],
				pixels[x + 2],
				pixels[x + 3]
			});
	}

	stbi_image_free(pixels);
}

Image::~Image()
{
}

void Image::SetPixel(glm::ivec2 position, glm::vec3 Color)
{
	SetPixel(position, Pixel(Color));
}

void Image::SetPixel(glm::ivec2 position, glm::vec4 Color)
{
	SetPixel(position, Pixel(Color));
}

void Image::SetPixel(glm::ivec2 position, Pixel pixel)
{
	int pos = position.x + (position.y * Width);
	ImageData[pos] = pixel;
}

void Image::FlipVertically()
{
	std::vector<Pixel> TempImageData;
	TempImageData.resize(ImageData.size(), Pixel(0xFF, 0x00, 0x00));

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			const int SourcePos = x + (y * Width);
			const int DestPos = (Width - x) + (y * Width);
			TempImageData[DestPos] = ImageData[SourcePos];
		}
	}

	ImageData = TempImageData;
}

void Image::FlipHorizontally()
{
	std::vector<Pixel> TempImageData;
	TempImageData.resize(ImageData.size(), Pixel(0xFF, 0x00, 0x00));

	for (int x = 0; x < Width; x++)
	{
		for (int y = 0; y < Height; y++)
		{
			const int SourcePos = x + (y * Width);
			const int DestPos = ((Height - y) * Width) - (x);
			const int zx = x;
			const int zy = Height - y;
			TempImageData[DestPos] = ImageData[SourcePos];
		}
	}

	ImageData = TempImageData;
}

void Image::UpdateColor(Pixel pixel)
{ 
	for (int x = 0; x < ImageData.size(); x++)
	{
		ImageData[x] = pixel;
	}
}

int Image::GetWidth()
{
	return Width;
}

int Image::GetHeight()
{
	return Height;
}

Pixel Image::GetPixel(glm::ivec2 position)
{
	int pos = position.x + (position.y * Width);
	return ImageData[pos];
}

VkDeviceSize Image::GetImageSize()
{
	return ImageSize;
}

byte* Image::GetImageData()
{
	return &ImageData[0].Red;
}
