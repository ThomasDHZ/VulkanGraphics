#include "CubeMapTexture.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include "Image.h"

CubeMapTexture::CubeMapTexture() : Texture()
{
}

CubeMapTexture::CubeMapTexture(VulkanRenderer& Renderer, CubeMapLayout CubeMapFiles) : Texture(Renderer, TextureType::vkTextureCube)
{
	SetUpCubeMapImage(Renderer, CubeMapFiles);
	CreateImageView(Renderer);
	CreateTextureSampler(Renderer);
}

CubeMapTexture::~CubeMapTexture()
{
}

void CubeMapTexture::SetUpCubeMapImage(VulkanRenderer& Renderer, CubeMapLayout CubeMapFiles)
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
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, imageSize, 0, &data);

	for (int i = 0; i < 6; ++i)
	{
		memcpy(static_cast<char*>(data) + (i * layerSize), SkyBoxLayout[i].GetImageData(), static_cast<size_t>(layerSize));
	}

	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);


	CreateImage(Renderer);

	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	CopyBufferToImage(Renderer, stagingBuffer);
	TransitionImageLayout(Renderer, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);
}

void CubeMapTexture::CreateTextureSampler(VulkanRenderer& Renderer)
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

	Texture::CreateTextureSampler(Renderer, SamplerInfo);
}
