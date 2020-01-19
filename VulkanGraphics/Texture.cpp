#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	Device = device;
	PhysicalDevice = physicalDevice;
	CommandBuffer = commandBuffer;
	CommandPool = commandPool;
	GraphicsQueue = graphicsQueue;

	LoadImage(device, physicalDevice, commandBuffer, commandPool, graphicsQueue);
	CreateImageView(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
	CreateTextureSampler();
}

Texture::~Texture()
{
}

void Texture::LoadImage(VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> commandBuffer, VkCommandPool commandPool, VkQueue graphicsQueue)
{
	int Channels;

	stbi_uc* Pixels = stbi_load("C:/Users/ZZT/source/repos/VulkanGraphics/VulkanGraphics/texture/texture.jpg", &Width, &Height, &Channels, STBI_rgb_alpha);
	VkDeviceSize ImageSize = Width * Height * STBI_rgb_alpha;

	if (!Pixels)
	{
		throw std::runtime_error("Couldn't load texture.");
	}

	VkBuffer StagingBuffer;
	VkDeviceMemory StagingBufferMemory;
	CreateBuffer(ImageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, StagingBuffer, StagingBufferMemory);

	void* data;
	vkMapMemory(device, StagingBufferMemory, 0, ImageSize, 0, &data);
	memcpy(data, Pixels, static_cast<size_t>(ImageSize));
	vkUnmapMemory(device, StagingBufferMemory);

	stbi_image_free(Pixels);

	CreateImage(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	TransitionImageLayout(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(StagingBuffer);
	TransitionImageLayout(VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void Texture::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
	VkBufferCreateInfo BufferInfo = {};
	BufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	BufferInfo.size = size;
	BufferInfo.usage = usage;
	BufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkResult Result = vkCreateBuffer(Device, &BufferInfo, nullptr, &buffer);
	if (Result != VK_SUCCESS) {
		throw std::runtime_error("Couldn't create buffer.");
	}

	VkMemoryRequirements MemoryRequirements;
	vkGetBufferMemoryRequirements(Device, buffer, &MemoryRequirements);

	VkMemoryAllocateInfo AllocInfo = {};
	AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	AllocInfo.allocationSize = MemoryRequirements.size;
	AllocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(PhysicalDevice, MemoryRequirements.memoryTypeBits, properties);

	Result = vkAllocateMemory(Device, &AllocInfo, nullptr, &bufferMemory);
	if (Result != VK_SUCCESS) 
	{
		throw std::runtime_error("Couldn't allocate buffer memory.");
	}

	vkBindBufferMemory(Device, buffer, bufferMemory, 0);
}

void Texture::CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = Width;
	imageInfo.extent.height = Height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(Device, &imageInfo, nullptr, &TextureImage) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(Device, TextureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(Device, &allocInfo, nullptr, &TextureImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(Device, TextureImage, TextureImageMemory, 0);
}

void Texture::TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkPipelineStageFlags SourceStage;
	VkPipelineStageFlags DestinationStage;

	VkCommandBuffer CommandBuffer = VulkanBufferManager::beginSingleTimeCommands(Device, CommandPool);

	VkImageMemoryBarrier MemoryBarrier = {};
	MemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	MemoryBarrier.oldLayout = oldLayout;
	MemoryBarrier.newLayout = newLayout;
	MemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	MemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	MemoryBarrier.image = TextureImage;
	MemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	MemoryBarrier.subresourceRange.baseMipLevel = 0;
	MemoryBarrier.subresourceRange.levelCount = 1;
	MemoryBarrier.subresourceRange.baseArrayLayer = 0;
	MemoryBarrier.subresourceRange.layerCount = 1;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
	{
		MemoryBarrier.srcAccessMask = 0;
		MemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		SourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		DestinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
	{
		MemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		MemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		SourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		DestinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else {
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(CommandBuffer, SourceStage, DestinationStage, 0, 0, nullptr, 0, nullptr, 1, &MemoryBarrier);

	VulkanBufferManager::endSingleTimeCommands(Device, CommandBuffer, CommandPool, GraphicsQueue);
}

void Texture::CopyBufferToImage(VkBuffer buffer)
{
	VkCommandBuffer CommandBuffer = VulkanBufferManager::beginSingleTimeCommands(Device, CommandPool);

	VkBufferImageCopy Region = {};
	Region.bufferOffset = 0;
	Region.bufferRowLength = 0;
	Region.bufferImageHeight = 0;
	Region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	Region.imageSubresource.mipLevel = 0;
	Region.imageSubresource.baseArrayLayer = 0;
	Region.imageSubresource.layerCount = 1;
	Region.imageOffset = { 0, 0, 0 };
	Region.imageExtent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1 };

	vkCmdCopyBufferToImage(CommandBuffer, buffer, TextureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &Region);

	VulkanBufferManager::endSingleTimeCommands(Device, CommandBuffer, CommandPool, GraphicsQueue);
}

void Texture::CreateImageView(VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo ImageViewInfo = {};
	ImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ImageViewInfo.image = TextureImage;
	ImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ImageViewInfo.format = format;
	ImageViewInfo.subresourceRange.aspectMask = aspectFlags;
	ImageViewInfo.subresourceRange.baseMipLevel = 0;
	ImageViewInfo.subresourceRange.levelCount = 1;
	ImageViewInfo.subresourceRange.baseArrayLayer = 0;
	ImageViewInfo.subresourceRange.layerCount = 1;

	VkResult Result = vkCreateImageView(Device, &ImageViewInfo, nullptr, &TextureImageView);
	if (Result != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void Texture::CreateTextureSampler()
{
	VkSamplerCreateInfo TextureSamplerInfo = {};
	TextureSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureSamplerInfo.magFilter = VK_FILTER_LINEAR;
	TextureSamplerInfo.minFilter = VK_FILTER_LINEAR;
	TextureSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureSamplerInfo.magFilter = VK_FILTER_LINEAR;
	TextureSamplerInfo.minFilter = VK_FILTER_LINEAR;
	TextureSamplerInfo.anisotropyEnable = VK_TRUE;
	TextureSamplerInfo.maxAnisotropy = 16;
	TextureSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	TextureSamplerInfo.unnormalizedCoordinates = VK_FALSE;
	TextureSamplerInfo.compareEnable = VK_FALSE;
	TextureSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	TextureSamplerInfo.mipLodBias = 0.0f;
	TextureSamplerInfo.minLod = 0.0f;
	TextureSamplerInfo.maxLod = 0.0f;

	VkResult Result = vkCreateSampler(Device, &TextureSamplerInfo, nullptr, &TextureSampler);
	if (Result != VK_SUCCESS)
	{
		throw std::runtime_error("Couldn't create texture sampler.");
	}
}

void Texture::CreateDescriptorSetLayout()
{
}

uint32_t Texture::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(PhysicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Texture::CleanUp()
{
	vkDestroySampler(Device, TextureSampler, nullptr);
	vkDestroyImageView(Device, TextureImageView, nullptr);
	vkDestroyImage(Device, TextureImage, nullptr);
	vkFreeMemory(Device, TextureImageMemory, nullptr);
}
