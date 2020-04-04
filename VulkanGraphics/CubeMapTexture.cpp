#include "CubeMapTexture.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include "Image.h"

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
	std::vector<Image> SkyBoxLayout;
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Left));
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Right));
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Top));
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Bottom));
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Back));
	SkyBoxLayout.emplace_back(Image(CubeMapFiles.Front));

	SkyBoxLayout[2].FlipHorizontally();
	SkyBoxLayout[3].FlipHorizontally();

	Width = SkyBoxLayout[0].GetWidth();
	Height = SkyBoxLayout[0].GetHeight();

	const VkDeviceSize imageSize = SkyBoxLayout[0].GetImageSize() * 6;
	const VkDeviceSize layerSize = imageSize / 6;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, imageSize, 0, &data);

	for (int i = 0; i < 6; ++i)
	{
		memcpy(static_cast<char*>(data) + (i * layerSize), SkyBoxLayout[i].GetImageData(), static_cast<size_t>(layerSize));
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

	Texture::CreateTextureSampler(SamplerInfo);
}
