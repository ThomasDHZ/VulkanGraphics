#pragma once
#include "Structs.h"
#include "Pixel.h"
#include <string>

class Texture
{
protected:
	enum TextureType
	{
		vkTexture2D,
		vkTextureCube
	};

	std::string FileName;
	int Width;
	int Height;

	VulkanDevice DeviceInfo;
	TextureType TypeOfTexture;

	void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer);
	void CreateImage();

public:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture();
	Texture(VulkanDevice deviceInfo, TextureType textureType);
	~Texture();

	void CreateImageView();
	void CreateTextureSampler(VkSamplerCreateInfo SamplerInfo);
	void Destroy();

	std::string GetTextureName() { return FileName; }
};

