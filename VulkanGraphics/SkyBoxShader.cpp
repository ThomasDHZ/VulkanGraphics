#include "SkyBoxShader.h"

SkyBoxShader::SkyBoxShader() : BaseShader()
{
}

SkyBoxShader::SkyBoxShader(VulkanDevice deviceInfo) : BaseShader(deviceInfo)
{
	CreateDescriptorSetLayout();
}

SkyBoxShader::~SkyBoxShader()
{
}

void SkyBoxShader::CreateDescriptorSetLayout()
{
	std::array<DescriptorSetLayoutBindingInfo, 5> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	BaseShader::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void SkyBoxShader::CreateShaderPipeLine()
{
}

void SkyBoxShader::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(SkyBoxUniformBufferObject);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void SkyBoxShader::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBoxShader::CreateDescriptorSets(VkImageView textureImageView, VkSampler textureSampler)
{
	//VkDescriptorImageInfo imageInfo = {};
	//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//imageInfo.imageView = textureImageView;
	//imageInfo.sampler = textureSampler;

	//VkDescriptorImageInfo imageInfo2 = {};
	//imageInfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//imageInfo2.imageView = textureImageView2;
	//imageInfo2.sampler = textureSampler2;

	//VkDescriptorImageInfo CubeMapInfo = {};
	//CubeMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//CubeMapInfo.imageView = skybox.textureImageView;
	//CubeMapInfo.sampler = skybox.textureSampler;

	//for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	//{
	//	VkDescriptorBufferInfo bufferInfo = {};
	//	bufferInfo.buffer = uniformBuffers[i];
	//	bufferInfo.offset = 0;
	//	bufferInfo.range = sizeof(UniformBufferObject2);

	//	VkDescriptorBufferInfo FragmentBufferInfo = {};
	//	FragmentBufferInfo.buffer = FragmentUniformBuffers[i];
	//	FragmentBufferInfo.offset = 0;
	//	FragmentBufferInfo.range = sizeof(FragmentUniformBufferObject);

	//	std::array<WriteDescriptorSetInfo, 5>  WriteDescriptorInfo = {};

	//	WriteDescriptorInfo[0].DstBinding = 0;
	//	WriteDescriptorInfo[0].DstSet = descriptorSets[i];
	//	WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

	//	WriteDescriptorInfo[1].DstBinding = 1;
	//	WriteDescriptorInfo[1].DstSet = descriptorSets[i];
	//	WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//	WriteDescriptorInfo[1].DescriptorImageInfo = imageInfo;

	//	WriteDescriptorInfo[2].DstBinding = 2;
	//	WriteDescriptorInfo[2].DstSet = descriptorSets[i];
	//	WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//	WriteDescriptorInfo[2].DescriptorImageInfo = imageInfo2;

	//	WriteDescriptorInfo[3].DstBinding = 3;
	//	WriteDescriptorInfo[3].DstSet = descriptorSets[i];
	//	WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//	WriteDescriptorInfo[3].DescriptorImageInfo = CubeMapInfo;

	//	WriteDescriptorInfo[4].DstBinding = 4;
	//	WriteDescriptorInfo[4].DstSet = descriptorSets[i];
	//	WriteDescriptorInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	WriteDescriptorInfo[4].DescriptorBufferInfo = FragmentBufferInfo;

	//	BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	//}


	BaseShader::CreateDescriptorSets();

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}
