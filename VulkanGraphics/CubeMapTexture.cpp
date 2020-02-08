#include "CubeMapTexture.h"
#include <stb_image.h>

CubeMapTexture::CubeMapTexture() : Texture()
{
}

CubeMapTexture::CubeMapTexture(VulkanDevice deviceInfo, CubeMapLayout CubeMapFiles) : Texture(deviceInfo, TextureType::vkTextureCube)
{
	SetUpCubeMapImage(CubeMapFiles);
	CreateImageView();
	CreateTextureSampler();
}

CubeMapTexture::~CubeMapTexture()
{
}

void CubeMapTexture::SetUpCubeMapImage(CubeMapLayout CubeMapFiles)
{
	unsigned char* textureData[6];
	int texChannels;

	textureData[0] = stbi_load(CubeMapFiles.Left.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	textureData[1] = stbi_load(CubeMapFiles.Right.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	textureData[2] = stbi_load(CubeMapFiles.Top.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	textureData[3] = stbi_load(CubeMapFiles.Bottom.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	textureData[4] = stbi_load(CubeMapFiles.Back.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);
	textureData[5] = stbi_load(CubeMapFiles.Front.c_str(), &Width, &Height, &texChannels, STBI_rgb_alpha);

	const VkDeviceSize imageSize = Width * Height * 4 * 6;
	const VkDeviceSize layerSize = imageSize / 6;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);

	for (int i = 0; i < 6; ++i)
	{
		memcpy(static_cast<char*>(data) + (i * layerSize), textureData[i], static_cast<size_t>(layerSize));
	}

	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);


	CreateImage();

	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(stagingBuffer);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void CubeMapTexture::CreateTextureSampler()
{
	VkSamplerCreateInfo SamplerInfo = {};
	SamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	SamplerInfo.magFilter = VK_FILTER_LINEAR;
	SamplerInfo.minFilter = VK_FILTER_LINEAR;
	SamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	SamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	SamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	SamplerInfo.anisotropyEnable = VK_TRUE;
	SamplerInfo.maxAnisotropy = 16;
	SamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	SamplerInfo.unnormalizedCoordinates = VK_FALSE;
	SamplerInfo.compareEnable = VK_FALSE;
	SamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	SamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

	Texture::CreateTextureSampler(SamplerInfo);
}
