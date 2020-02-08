#pragma once
#include "Mesh.h"
class Texture
{
protected:
	enum TextureType
	{
		vkTexture2D,
		vkTextureCube
	};

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
};

