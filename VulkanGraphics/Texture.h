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
	VulkanRenderer* renderer;
	TextureType TypeOfTexture;

	void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer);
	void CreateImage();

public:
	int Width;
	int Height;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture();
	Texture(VulkanRenderer* Renderer, TextureType textureType);
	~Texture();

	void Update();
	void CreateImageView();
	void CreateTextureSampler(VkSamplerCreateInfo SamplerInfo);
	void Destroy();

	std::string GetTextureName() { return FileName; }
};

