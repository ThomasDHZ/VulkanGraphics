#include "Texture2D.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(Renderer& renderer, std::string TexturePath) : Texture(renderer, TextureType::vkTexture2D)
{
	stbi_set_flip_vertically_on_load(true);

	stbi_uc* pixels = stbi_load(TexturePath.c_str(), &Width, &Height, &ColorChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * ColorChannels;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

	CreateImage(renderer);

	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(renderer, stagingBuffer);
	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);

	CreateImageView(renderer);
	CreateTextureSampler(renderer);
}

Texture2D::~Texture2D()
{
}

void Texture2D::UpdateTexture(Renderer& renderer, Pixel pixel)
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	memset(&PixelImage[0], 0xFF, imageSize);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(renderer, stagingBuffer);
	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);

	vkDestroyImageView(*GetDevice(renderer), textureImageView, nullptr);
	CreateImageView(renderer);
}

void Texture2D::UpdateTexture(Renderer& renderer)
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, &PixelImage[0], static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(renderer, stagingBuffer);
	TransitionImageLayout(renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);

	vkDestroyImageView(*GetDevice(renderer), textureImageView, nullptr);
	CreateImageView(renderer);
}

void Texture2D::CreateTextureSampler(Renderer& renderer)
{
	VkSamplerCreateInfo SamplerInfo = {};
	SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.magFilter = VK_FILTER_NEAREST;
	SamplerInfo.minFilter = VK_FILTER_NEAREST;
	SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	SamplerInfo.anisotropyEnable = VK_TRUE;
	SamplerInfo.maxAnisotropy = 16;
	SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	SamplerInfo.unnormalizedCoordinates = VK_FALSE;
	SamplerInfo.compareEnable = VK_FALSE;
	SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	Texture::CreateTextureSampler(renderer, SamplerInfo);
}