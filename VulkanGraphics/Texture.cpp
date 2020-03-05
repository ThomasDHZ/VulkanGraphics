#include "Texture.h"
#include "VulkanBufferManager.h"

Texture::Texture()
{
	FileName = "";
	Width = 0;
	Height = 0;
}

Texture::Texture(VulkanDevice deviceInfo, TextureType textureType)
{
	DeviceInfo = deviceInfo;
	TypeOfTexture = textureType;
}

Texture::~Texture()
{
}

void Texture::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(DeviceInfo.Device, DeviceInfo.CommandPool);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = textureImage;
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
	VulkanBufferManager::endSingleTimeCommands(DeviceInfo.Device, commandBuffer, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);
}

void Texture::CopyBufferToImage(VkBuffer buffer)
{
	VkCommandBuffer commandBuffer = VulkanBufferManager::beginSingleTimeCommands(DeviceInfo.Device, DeviceInfo.CommandPool);

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

	vkCmdCopyBufferToImage(commandBuffer, buffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
	VulkanBufferManager::endSingleTimeCommands(DeviceInfo.Device, commandBuffer, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);
}

void Texture::CreateImage()
{
	VkImageCreateInfo ImageInfo = {};
	ImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ImageInfo.imageType = VK_IMAGE_TYPE_2D;
	ImageInfo.extent.width = Width;
	ImageInfo.extent.height = Height;
	ImageInfo.extent.depth = 1;
	ImageInfo.mipLevels = 1;
	ImageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	ImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	ImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	ImageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	ImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	ImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (TypeOfTexture == TextureType::vkTexture2D)
	{
		ImageInfo.arrayLayers = 1;
	}
	else if (TypeOfTexture == TextureType::vkTextureCube)
	{
		ImageInfo.arrayLayers = 6;
		ImageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
	}

	if (vkCreateImage(DeviceInfo.Device, &ImageInfo, nullptr, &textureImage) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(DeviceInfo.Device, textureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(DeviceInfo.PhysicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(DeviceInfo.Device, &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(DeviceInfo.Device, textureImage, textureImageMemory, 0);
}

void Texture::CreateImageView()
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = textureImage;
	viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;

	if (TypeOfTexture == TextureType::vkTexture2D)
	{
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.subresourceRange.layerCount = 1;
	}
	else if (TypeOfTexture == TextureType::vkTextureCube)
	{
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		viewInfo.subresourceRange.layerCount = 6;
	}

	VkImageView imageView;
	if (vkCreateImageView(DeviceInfo.Device, &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void Texture::CreateTextureSampler(VkSamplerCreateInfo SamplerInfo)
{
	if (vkCreateSampler(DeviceInfo.Device, &SamplerInfo, nullptr, &textureSampler) != VK_SUCCESS) 
	{
		throw std::runtime_error("Failed to create texture sampler.");
	}
}

void Texture::Destroy()
{
	vkDestroySampler(DeviceInfo.Device, textureSampler, nullptr);
	vkDestroyImageView(DeviceInfo.Device, textureImageView, nullptr);

	vkDestroyImage(DeviceInfo.Device, textureImage, nullptr);
	vkFreeMemory(DeviceInfo.Device, textureImageMemory, nullptr);
}
