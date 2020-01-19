#pragma once
#include <vulkan/vulkan.h>

#include <stb_image.h>
#include <stdexcept>

#include "VulkanBufferManager.h"
#include <vector>

class Texture
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

	void LoadImage(std::string TexturePath, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer);
	void CreateTextureSampler();
	void CreateDescriptorSetLayout();

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
public:
	Texture();
	Texture(std::string TexturePath, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue);
	~Texture();

	void CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags);
	void CleanUp();

	VkImage GetTextureImage() { return TextureImage; }
	VkDeviceMemory GetTextureImageMemory() { return TextureImageMemory; }
	VkImageView GetTextureImageView() { return TextureImageView; }
	VkSampler GetTextureSampler() { return TextureSampler; }
};

