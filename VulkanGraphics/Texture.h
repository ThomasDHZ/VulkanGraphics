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

	void TransitionImageLayout(VulkanRenderer& Renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VulkanRenderer& Renderer, VkBuffer buffer);
	void CreateImage(VulkanRenderer& Renderer);

public:
	int Width;
	int Height;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture();
	Texture(VulkanRenderer& Renderer, TextureType textureType);
	~Texture();

	void CreateImageView(VulkanRenderer& Renderer);
	void CreateTextureSampler(VulkanRenderer& Renderer, VkSamplerCreateInfo SamplerInfo);
	void Destroy(VulkanRenderer& Renderer);

	std::string GetTextureName() { return FileName; }
};

