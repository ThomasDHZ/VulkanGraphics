#include "NewTexture2D.h"
#include "VulkanBufferManager.h"

NewTexture2D::NewTexture2D() : NewTexture()
{
}

NewTexture2D::NewTexture2D(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation) : NewTexture(Device, PhysicalDevice, CommandPool, GraphicsQueue, TextureLocation)
{
	LoadTexture(Device, PhysicalDevice, CommandPool, GraphicsQueue, TextureLocation);
	CreateTextureView(Device);
	CreateTextureSampler(Device);
}

NewTexture2D::~NewTexture2D()
{
}

void NewTexture2D::LoadTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation)
{
	int ColorChannels;
	stbi_uc* pixels = stbi_load(TextureLocation.c_str(), &Width, &Height, &ColorChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * 4;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(Device, PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	VkImageCreateInfo TextureInfo = {};
	TextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	TextureInfo.imageType = VK_IMAGE_TYPE_2D;
	TextureInfo.extent.width = Width;
	TextureInfo.extent.height = Height;
	TextureInfo.extent.depth = 1;
	TextureInfo.mipLevels = 1;
	TextureInfo.arrayLayers = 1;
	TextureInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	TextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	TextureInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	TextureInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	TextureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	TextureInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	void* data;
	vkMapMemory(Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(Device, stagingBufferMemory);

	NewTexture::CreateTextureImage(Device, PhysicalDevice, TextureInfo);

	TransitionImageLayout(Device, CommandPool, GraphicsQueue, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Device, CommandPool, GraphicsQueue, stagingBuffer);
	TransitionImageLayout(Device, CommandPool, GraphicsQueue, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(Device, stagingBuffer, nullptr);
	vkFreeMemory(Device, stagingBufferMemory, nullptr);
}

void NewTexture2D::CreateTextureView(VkDevice Device)
{
	VkImageViewCreateInfo TextureImageViewInfo = {};
	TextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	TextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	TextureImageViewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
	TextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	TextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	TextureImageViewInfo.subresourceRange.levelCount = 1;
	TextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	TextureImageViewInfo.subresourceRange.layerCount = 1;
	TextureImageViewInfo.image = Image;

	NewTexture::CreateTextureView(Device, TextureImageViewInfo);
}

void NewTexture2D::CreateTextureSampler(VkDevice Device)
{
	VkSamplerCreateInfo TextureImageSamplerInfo = {};
	TextureImageSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	TextureImageSamplerInfo.magFilter = VK_FILTER_LINEAR;
	TextureImageSamplerInfo.minFilter = VK_FILTER_LINEAR;
	TextureImageSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	TextureImageSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	TextureImageSamplerInfo.mipLodBias = 0.0f;
	TextureImageSamplerInfo.maxAnisotropy = 16.0f;
	TextureImageSamplerInfo.minLod = 0.0f;
	TextureImageSamplerInfo.maxLod = 1.0f;
	TextureImageSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
	TextureImageSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	TextureImageSamplerInfo.anisotropyEnable = VK_TRUE;

	NewTexture::CreateTextureSampler(Device, TextureImageSamplerInfo);
}