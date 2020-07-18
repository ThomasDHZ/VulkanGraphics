#include "NewTexture.h"
#include <stdexcept>

#include "VulkanBufferManager.h"

NewTexture::NewTexture()
{
}

NewTexture::NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation)
{
}

NewTexture::NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec2 TextureSize)
{
}

NewTexture::NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec3 TextureSize)
{
}

NewTexture::~NewTexture()
{
}

void NewTexture::TransitionImageLayout(VkDevice Device, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(Device, CommandPool);

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

	//if (TypeOfTexture == TextureType::vkTexture2D)
	//{
	barrier.subresourceRange.layerCount = 1;
	//}
	//else if (TypeOfTexture == TextureType::vkTextureCube)
	//{
	//	barrier.subresourceRange.layerCount = 6;
	//}

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
	VulkanBufferManager::endSingleTimeCommands(Device, commandBuffer, CommandPool, GraphicsQueue);
}

void NewTexture::CopyBufferToImage(VkDevice Device, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkBuffer buffer)
{
	VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(Device, CommandPool);

	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1 };

	//if (TypeOfTexture == TextureType::vkTexture2D)
	//{
	region.imageSubresource.layerCount = 1;
	//}
	//else if (TypeOfTexture == TextureType::vkTextureCube)
	//{
	//	region.imageSubresource.layerCount = 6;
	/*}*/

	vkCmdCopyBufferToImage(commandBuffer, buffer, Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	VulkanBufferManager::endSingleTimeCommands(Device, commandBuffer, CommandPool, GraphicsQueue);
}

void NewTexture::CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkImageCreateInfo TextureInfo)
{
	if (vkCreateImage(Device, &TextureInfo, nullptr, &Image)) {
		throw std::runtime_error("Failed to create Image.");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(Device, Image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(PhysicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(Device, &allocInfo, nullptr, &Memory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate image Memory.");
	}

	vkBindImageMemory(Device, Image, Memory, 0);
}

void NewTexture::CreateTextureView(VkDevice Device, VkImageViewCreateInfo TextureImageViewInfo)
{
	if (vkCreateImageView(Device, &TextureImageViewInfo, nullptr, &View)) {
		throw std::runtime_error("Failed to create Image View.");
	}
}

void NewTexture::CreateTextureSampler(VkDevice Device, VkSamplerCreateInfo TextureImageSamplerInfo)
{
	if (vkCreateSampler(Device, &TextureImageSamplerInfo, nullptr, &Sampler))
	{
		throw std::runtime_error("Failed to create Sampler.");
	}
}
