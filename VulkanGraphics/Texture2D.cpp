#include "Texture2D.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(VulkanDevice deviceInfo, std::string TexturePath) : Texture(deviceInfo, TextureType::vkTexture2D)
{
	TextureImage = Image(TexturePath);
	Width = TextureImage.GetWidth();
	Height = TextureImage.GetHeight();
	VkDeviceSize imageSize = TextureImage.GetImageSize();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, TextureImage.GetImageData(), static_cast<size_t>(imageSize));
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	CreateImage();

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);

	CreateImageView();
	CreateTextureSampler();
}

Texture2D::Texture2D(VulkanDevice deviceInfo, int width, int height, Pixel TextureColor) : Texture(deviceInfo, TextureType::vkTexture2D)
{
	TextureImage = Image(width, height, TextureColor);
	Width = TextureImage.GetWidth();
	Height = TextureImage.GetHeight();
	VkDeviceSize imageSize = TextureImage.GetImageSize();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, TextureImage.GetImageData(), static_cast<size_t>(imageSize));
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	CreateImage();

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);

	CreateImageView();
	CreateTextureSampler();
}

Texture2D::~Texture2D()
{
}

void Texture2D::CreateTextureSampler()
{
	VkSamplerCreateInfo SamplerInfo = {};
	SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.magFilter = VK_FILTER_LINEAR;
	SamplerInfo.minFilter = VK_FILTER_LINEAR;
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

	Texture::CreateTextureSampler(SamplerInfo);
}

void Texture2D::SetPixel(glm::ivec2 position, glm::vec3 color)
{
	TextureImage.SetPixel(position, color);
}

void Texture2D::SetPixel(glm::ivec2 position, glm::vec4 color)
{
	TextureImage.SetPixel(position, color);
}

void Texture2D::SetPixel(glm::ivec2 position, Pixel pixel)
{
	TextureImage.SetPixel(position, pixel);
}

void Texture2D::UpdateTexture()
{
	VkDeviceSize imageSize = TextureImage.GetImageSize();
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, TextureImage.GetImageData(), static_cast<size_t>(imageSize));
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	CreateImage();

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);

	CreateImageView();
}

Pixel Texture2D::GetPixel(glm::ivec2 position)
{
	return TextureImage.GetPixel(position);
}

VkDeviceSize Texture2D::GetImageSize()
{
	return TextureImage.GetImageSize();
}
