#pragma once

#include "Structs.h"
#include "Pixel.h"
#include "VulkanResources.h"
#include <string>

class Texture : protected VulkanResources
{
protected:
	enum TextureType
	{
		vkTexture2D,
		vkTextureCube
	};

	std::string FileName;
	TextureType TypeOfTexture;

	void TransitionImageLayout(Renderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(Renderer& renderer, VkBuffer buffer);
	void CreateImage(Renderer& renderer);

public:
	int Width;
	int Height;

	VkImage textureImage = VK_NULL_HANDLE;
	VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;

	Texture();
	Texture(Renderer& Renderer, TextureType textureType);
	~Texture();

	void CreateImageView(Renderer& renderer);
	void CreateTextureSampler(Renderer& renderer, VkSamplerCreateInfo SamplerInfo);
	void Destroy(Renderer& renderer);

	std::string GetTextureName() { return FileName; }
};

