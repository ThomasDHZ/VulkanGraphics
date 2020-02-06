#pragma once
#include <vulkan\vulkan_core.h>
#include "Mesh.h"

class Texture
{
private:
	int Width; 
	int Height;

	VulkanDevice DeviceInfo;

	void CreateTextureImage(std::string TexturePath);
	void CreateImageView();
	void CreateTextureSampler();

	void CreateImage();
	void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer);
public:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture();
	Texture(VulkanDevice deviceInfo, std::string TexturePath);
	~Texture();

	void Destroy();
};

