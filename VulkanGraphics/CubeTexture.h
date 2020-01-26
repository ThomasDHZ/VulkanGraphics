#pragma once
#include <vulkan/vulkan.h>

#include <stb_image.h>
#include <stdexcept>

#include "VulkanBufferManager.h"
#include <vector>

class CubeTexture
{
private:

	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	std::vector<VkCommandBuffer> CommandBuffer;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;

	int Width;
	int Height;
	VkImage TextureImage;
	VkDeviceMemory TextureImageMemory;
	VkImageView TextureImageView;
	VkSampler TextureSampler;
	VkDeviceSize ImageSize;

	void LoadCubeMap(std::vector<std::string> CubeMapPaths, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer);
	void CreateTextureSampler();
	void CreateDescriptorSetLayout();

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
public:
	CubeTexture();
	CubeTexture(std::vector<std::string> CubeMapPaths, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);
	~CubeTexture();

	void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
	void Destory();

	VkImageView GetTextureImageView() { return TextureImageView; }
	VkSampler GetTextureSampler() { return TextureSampler; }
};
