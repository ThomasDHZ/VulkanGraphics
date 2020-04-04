#pragma once
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <vulkan\vulkan_core.h>
#include "Pixel.h"

class Image
{
private:
	int Width;
	int Height;
	unsigned int ImageSize;
	std::vector<Pixel> ImageData;

public:
	Image();
	Image(std::string TexturePath);
	~Image();

	void SetPixel(glm::ivec2 position, glm::vec3 Color);
	void SetPixel(glm::ivec2 position, glm::vec4 Color);
	void SetPixel(glm::ivec2 position, Pixel pixel);

	void FlipVertically();
	void FlipHorizontally();
	void UpdateColor(Pixel pixel);

	int GetWidth();
	int GetHeight();
	Pixel GetPixel(glm::ivec2 position);
	VkDeviceSize GetImageSize();
	byte* GetImageData();
};

