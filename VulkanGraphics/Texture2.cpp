#include "Texture2.h"
#include <stdexcept>

#include "VulkanBufferManager.h"
#include "NewVulkanBufferManager.h"

Texture2::Texture2()
{
	FileName = "";
	Width = 0;
	Height = 0;
}

Texture2::Texture2(VulkanRenderer& renderer, std::string TextureLocation, TextureType textureType)
{
	TypeOfTexture = textureType;
}

Texture2::Texture2(VulkanRenderer& renderer, TextureType textureType)
{
	TypeOfTexture = textureType;
}

Texture2::~Texture2()
{
}

void Texture2::TransitionImageLayout(VulkanRenderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = NewVulkanBufferManager::beginSingleTimeCommands(renderer);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = Image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;

	if (TypeOfTexture == TextureType::vkTexture2D)
	{
		barrier.subresourceRange.layerCount = 1;
	}
	else if (TypeOfTexture == TextureType::vkTextureCube)
	{
		barrier.subresourceRange.layerCount = 6;
	}

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	NewVulkanBufferManager::endSingleTimeCommands(renderer, commandBuffer);
}

void Texture2::CopyBufferToImage(VulkanRenderer& renderer, VkBuffer buffer)
{
	VkCommandBuffer commandBuffer = NewVulkanBufferManager::beginSingleTimeCommands(renderer);

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1 };

	if (TypeOfTexture == TextureType::vkTexture2D)
	{
		region.imageSubresource.layerCount = 1;
	}
	else if (TypeOfTexture == TextureType::vkTextureCube)
	{
		region.imageSubresource.layerCount = 6;
	}

	vkCmdCopyBufferToImage(commandBuffer, buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	NewVulkanBufferManager::endSingleTimeCommands(renderer, commandBuffer);
}

void Texture2::CreateTextureImage(VulkanRenderer& renderer, VkImageCreateInfo TextureInfo)
{
	if (vkCreateImage(renderer.Device, &TextureInfo, nullptr, &Image)) {
		throw std::runtime_error("Failed to create Image.");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(renderer.Device, Image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = NewVulkanBufferManager::FindMemoryType(renderer, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(renderer.Device, &allocInfo, nullptr, &Memory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate image Memory.");
	}

	vkBindImageMemory(renderer.Device, Image, Memory, 0);
}

void Texture2::CreateTextureView(VulkanRenderer& renderer, VkImageViewCreateInfo TextureImageViewInfo)
{
	if (vkCreateImageView(renderer.Device, &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void Texture2::CreateTextureSampler(VulkanRenderer& renderer, VkSamplerCreateInfo TextureImageSamplerInfo)
{
	if (vkCreateSampler(renderer.Device, &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}
}

void Texture2::Delete(VulkanRenderer& renderer)
{
	vkDestroyImageView(renderer.Device, View, nullptr);
	vkDestroyImage(renderer.Device, Image, nullptr);
	vkFreeMemory(renderer.Device, Memory, nullptr);
	vkDestroySampler(renderer.Device, Sampler, nullptr);

	View = VK_NULL_HANDLE;
	Image = VK_NULL_HANDLE;
	Memory = VK_NULL_HANDLE;
	Sampler = VK_NULL_HANDLE;
}
