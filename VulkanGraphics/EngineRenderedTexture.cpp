#include "EngineRenderedTexture.h"

EngineRenderedTexture::EngineRenderedTexture() : Texture()
{
}

EngineRenderedTexture::EngineRenderedTexture(Renderer& renderer, VkFormat ImageFormat, VkExtent2D TextureSize) : Texture(renderer, TextureType::vkTexture2D)
{
	Width = TextureSize.width;
	Height = TextureSize.height;

	CreateEngineRendereredImageHolder(renderer, ImageFormat);
	CreateEngineRendereredImageView(renderer, ImageFormat);
	CreateEngineRendereredImageSampler(renderer);
}

EngineRenderedTexture::~EngineRenderedTexture()
{
}

void EngineRenderedTexture::CreateEngineRendereredImageHolder(Renderer& renderer, VkFormat ImageFormat)
{
	VkImageCreateInfo RenderedTextureInfo = {};
	RenderedTextureInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	RenderedTextureInfo.imageType = VK_IMAGE_TYPE_2D;
	RenderedTextureInfo.format = ImageFormat;
	RenderedTextureInfo.extent.width = Width;
	RenderedTextureInfo.extent.height = Height;
	RenderedTextureInfo.extent.depth = 1;
	RenderedTextureInfo.mipLevels = 1;
	RenderedTextureInfo.arrayLayers = 1;
	RenderedTextureInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	RenderedTextureInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	RenderedTextureInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	if (vkCreateImage(*GetDevice(renderer), &RenderedTextureInfo, nullptr, &textureImage)) {
		throw std::runtime_error("failed to create vkCreateImage!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(*GetDevice(renderer), textureImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(*GetPhysicalDevice(renderer), memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(*GetDevice(renderer), &allocInfo, nullptr, &textureImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(*GetDevice(renderer), textureImage, textureImageMemory, 0);
}

void EngineRenderedTexture::CreateEngineRendereredImageView(Renderer& renderer, VkFormat ImageFormat)
{
	VkImageViewCreateInfo RenderedTextureImageViewInfo = {};
	RenderedTextureImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	RenderedTextureImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	RenderedTextureImageViewInfo.format = ImageFormat;
	RenderedTextureImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	RenderedTextureImageViewInfo.subresourceRange.baseMipLevel = 0;
	RenderedTextureImageViewInfo.subresourceRange.levelCount = 1;
	RenderedTextureImageViewInfo.subresourceRange.baseArrayLayer = 0;
	RenderedTextureImageViewInfo.subresourceRange.layerCount = 1;
	RenderedTextureImageViewInfo.image = textureImage;
	if (vkCreateImageView(*GetDevice(renderer), &RenderedTextureImageViewInfo, nullptr, &textureImageView)) {
		throw std::runtime_error("failed to create vkCreateImageView!");
	}
}

void EngineRenderedTexture::CreateEngineRendereredImageSampler(Renderer& renderer)
{
	VkSamplerCreateInfo RenderedTextureImageInfo = {};
	RenderedTextureImageInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	RenderedTextureImageInfo.magFilter = VK_FILTER_LINEAR;
	RenderedTextureImageInfo.minFilter = VK_FILTER_LINEAR;
	RenderedTextureImageInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	RenderedTextureImageInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	RenderedTextureImageInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	RenderedTextureImageInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	RenderedTextureImageInfo.mipLodBias = 0.0f;
	RenderedTextureImageInfo.maxAnisotropy = 1.0f;
	RenderedTextureImageInfo.minLod = 0.0f;
	RenderedTextureImageInfo.maxLod = 1.0f;
	RenderedTextureImageInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
	if (vkCreateSampler(*GetDevice(renderer), &RenderedTextureImageInfo, nullptr, &textureSampler))
	{
		throw std::runtime_error("failed to create vkCreateSampler!");
	}
}
