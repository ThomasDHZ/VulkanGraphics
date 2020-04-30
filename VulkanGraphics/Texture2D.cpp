#include "Texture2D.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"

Texture2D::Texture2D() : Texture()
{
}

Texture2D::Texture2D(VulkanRenderer& Renderer, std::string TexturePath) : Texture(Renderer, TextureType::vkTexture2D)
{
	int texChannels;
	stbi_uc* pixels = stbi_load(TexturePath.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = Width * Height * 4;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	CreateImage(Renderer);

	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Renderer, stagingBuffer);
	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);

	CreateImageView(Renderer);
	CreateTextureSampler(Renderer);
}

Texture2D::Texture2D(VulkanRenderer& Renderer, int width, int height, Pixel TextureColor) : Texture(Renderer, TextureType::vkTexture2D)
{
	Width = width;
	Height = height;

	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	PixelImage.resize(Width * Height, Pixel(TextureColor));

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	CreateImage(Renderer);

	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Renderer, stagingBuffer);
	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);

	CreateImageView(Renderer);
	CreateTextureSampler(Renderer);
}

Texture2D::~Texture2D()
{
}

void Texture2D::UpdateTexture(VulkanRenderer& Renderer, Pixel pixel)
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	memset(&PixelImage[0], 0xFF, imageSize);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Renderer, stagingBuffer);
	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);

	vkDestroyImageView(*GetDevice(Renderer), textureImageView, nullptr);
	CreateImageView(Renderer);
}

void Texture2D::UpdateTexture(VulkanRenderer& Renderer)
{
	VkDeviceSize imageSize = Width * Height * sizeof(Pixel);
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, &PixelImage[0], static_cast<size_t>(imageSize));
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Renderer, stagingBuffer);
	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);

	vkDestroyImageView(*GetDevice(Renderer), textureImageView, nullptr);
	CreateImageView(Renderer);
}

void Texture2D::CreateTextureSampler(VulkanRenderer& Renderer)
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

	Texture::CreateTextureSampler(Renderer, SamplerInfo);
}

void Texture2D::SetPixel(VulkanRenderer& Renderer, glm::ivec2 pos, Pixel pixel)
{
	const int position = pos.x + (pos.y * Width);
	memcpy(&PixelImage[position], &pixel, sizeof(Pixel));
	UpdateTexture(Renderer);
}

void Texture2D::CopyRange(VulkanRenderer& Renderer, const Texture2D& texture, int CopyWidth, int CopyHeight)
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
	UpdateTexture(Renderer);
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
