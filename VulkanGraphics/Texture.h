#pragma once

#include "Structs.h"
#include "Pixel.h"
#include <string>
#include "VulkanRenderer.h"

class Texture
{
protected:
	enum TextureType
	{
		vkTexture2D,
		vkTextureCube
	};

	std::string FileName;
	TextureType TypeOfTexture;


	void TransitionImageLayout(VulkanRenderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VulkanRenderer& renderer, VkBuffer buffer);
	void CreateImage(VulkanRenderer& renderer);

public:
	int Width;
	int Height;
	int ColorChannels;

	VkImage textureImage = VK_NULL_HANDLE;
	VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;

	Texture();
	Texture(VulkanRenderer& Renderer, TextureType textureType);
	~Texture();

	void CreateImageView(VulkanRenderer& renderer);
	void CreateTextureSampler(VulkanRenderer& renderer, VkSamplerCreateInfo SamplerInfo);
	void Destroy(VulkanRenderer& renderer);

	std::string GetTextureName() { return FileName; }
};

