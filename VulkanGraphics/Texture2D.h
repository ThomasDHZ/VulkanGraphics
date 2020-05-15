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

	void CreateTextureSampler(VulkanRenderer& Renderer);
public:
	std::vector<Pixel> PixelImage;

	Texture2D();
	Texture2D(VulkanRenderer& Renderer, std::string TexturePath);
	Texture2D(VulkanRenderer& Renderer, int width, int height, Pixel TextureColor);
	~Texture2D();

	void SetPixel(VulkanRenderer& Renderer, glm::ivec2 pos, Pixel pixel);
	void CopyRange(VulkanRenderer& Renderer, const Texture2D& texture, int CopyWidth, int CopyHeight);
	void UpdateTexture(VulkanRenderer& Renderer, Pixel pixel);
	void UpdateTexture(VulkanRenderer& Renderer);

	Pixel GetPixel(glm::ivec2 pos);
	void* GetPixelPtr(glm::ivec2 pos);
};

