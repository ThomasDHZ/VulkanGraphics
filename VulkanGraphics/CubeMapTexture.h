#pragma once
#include "Mesh.h"
class CubeMapTexture
{
private:
	VulkanDevice DeviceInfo;
public:
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	CubeMapTexture();
	CubeMapTexture(VulkanDevice deviceInfo);
	~CubeMapTexture();
	void SetUpCubeMapImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void TransitionCubeMapImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToCubeMapImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void SetUpCubeMapImage();
	void SetUpSkyBoxImageView();
	void SetUpCubeMapSampler();
	void Destroy();
};

