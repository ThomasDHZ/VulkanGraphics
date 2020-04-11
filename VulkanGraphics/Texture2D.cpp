#include "Texture2D.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(VulkanDevice deviceInfo, std::string TexturePath) : Texture(deviceInfo, TextureType::vkTexture2D)
{
	int texChannels;
	stbi_uc* pixels = stbi_load(TexturePath.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * 4;

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}

	for (int x = 0; x <= imageSize; x += 4)
	{
		PixelImage.emplace_back(Pixel
			{
				pixels[x],
				pixels[x + 1],
				pixels[x + 2],
				pixels[x + 3]
			});
	}

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
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
	Width = width;
	Height = height;

	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	PixelImage.resize(Width * Height, Pixel(TextureColor));

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
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

void Texture2D::UpdateTexture(Pixel pixel)
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	PixelImage.clear();
	PixelImage.resize(Width * Height, pixel);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);

	vkDestroyImageView(DeviceInfo.Device, textureImageView, nullptr);
	CreateImageView();
}

void Texture2D::UpdateTexture()
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, &PixelImage[0], static_cast<size_t>(imageSize));
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);

	vkDestroyImageView(DeviceInfo.Device, textureImageView, nullptr);
	CreateImageView();
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

void Texture2D::SetPixel(glm::ivec2 pos, Pixel pixel)
{
	const int position = pos.x + (pos.y * Width);
	memcpy(&PixelImage[position], &pixel, sizeof(Pixel));
	UpdateTexture();
}

void Texture2D::CopyRange(const Texture2D& texture, int CopyWidth, int CopyHeight)
{
	if (Width > texture.Width)
	{
		for (int x = 0; x < texture.Height; x++)
		{
			const int dstPos = CopyWidth + ((CopyHeight + x) * Width);
			const int SourcePos = 0 + ((0 + x) * texture.Width);
			memcpy(&PixelImage[dstPos], &texture.PixelImage[SourcePos], sizeof(Pixel) * texture.Width);
		}
	}
	else
	{
		for (int x = 0; x < Height; x++)
		{
			const int dstPos = 0 + (x * Width);
			const int SourcePos = CopyWidth + ((CopyHeight + x) * texture.Width);
			memcpy(&PixelImage[dstPos], &texture.PixelImage[SourcePos], sizeof(Pixel) * Width);
		}
	}
	UpdateTexture();
}

Pixel Texture2D::GetPixel(glm::ivec2 pos)
{
	const int position = pos.x + (pos.y * Width);
	return PixelImage[position];
}

void* Texture2D::GetPixelPtr(glm::ivec2 pos)
{
	const int position = pos.x + (pos.y * Width);
	return &PixelImage[position].Red;
}
