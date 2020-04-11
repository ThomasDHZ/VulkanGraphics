#pragma once
#include <vulkan\vulkan_core.h>
#include <array>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include "Texture.h"
#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include "Image.h"

class Texture2D : public Texture
{
private:
	VkDeviceSize imageSize;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	void CreateTextureSampler();
public:
	std::vector<Pixel> PixelImage;

	Texture2D();
	Texture2D(VulkanDevice deviceInfo, std::string TexturePath);
	Texture2D(VulkanDevice deviceInfo, int width, int height, Pixel TextureColor);
	~Texture2D();

	void SetPixel(glm::ivec2 pos, Pixel pixel);
	void CopyRange(const Texture2D& texture);
	void UpdateTexture(Pixel pixel);
	void UpdateTexture();

	Pixel GetPixel(glm::ivec2 pos);
	void* GetPixelPtr(glm::ivec2 pos);
};

