#include "Shader.h"

Shader::Shader() : BaseShader()
{
}

Shader::Shader(VulkanDevice deviceInfo) : BaseShader(deviceInfo)
{
	CreateDescriptorSetLayout();
}

Shader::~Shader()
{
}

void Shader::CreateDescriptorSetLayout()
{
	std::array<DescriptorSetLayoutBindingInfo, 5> LayoutBindingInfo = {};

	LayoutBindingInfo[0].Binding = 0;
	LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	LayoutBindingInfo[1].Binding = 1;
	LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[2].Binding = 2;
	LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[3].Binding = 3;
	LayoutBindingInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	LayoutBindingInfo[3].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	LayoutBindingInfo[4].Binding = 4;
	LayoutBindingInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	LayoutBindingInfo[4].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	
	BaseShader::CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end()));
}

void Shader::CreateShaderPipeLine()
{
}

void Shader::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject2);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}

	VkDeviceSize FragmentbufferSize = sizeof(FragmentUniformBufferObject);

	FragmentUniformBuffers.resize(DeviceInfo.SwapChainSize);
	FragmentUniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++) {
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, FragmentbufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, FragmentUniformBuffers[i], FragmentUniformBuffersMemory[i]);
	}
}

void Shader::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 5>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Shader::CreateDescriptorSets(SkyBox skybox, VkImageView textureImageView, VkSampler textureSampler, VkImageView textureImageView2, VkSampler textureSampler2)
{

	//BaseShader::CreateDescriptorSets();

	//VkDescriptorImageInfo imageInfo = {};
	//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//imageInfo.imageView = textureImageView;
	//imageInfo.sampler = textureSampler;

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

	//	std::array<WriteDescriptorSetInfo, 4> WriteDescriptorInfo = {};
	//	WriteDescriptorInfo[0].DstBinding = 0;
	//	WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

	//	WriteDescriptorInfo[1].DstBinding = 1;
	//	WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//	WriteDescriptorInfo[1].DescriptorImageInfo = imageInfo;

	//	WriteDescriptorInfo[2].DstBinding = 2;
	//	WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//	WriteDescriptorInfo[2].DescriptorImageInfo = CubeMapInfo;

	//	WriteDescriptorInfo[3].DstBinding = 3;
	//	WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//	WriteDescriptorInfo[3].DescriptorBufferInfo = FragmentBufferInfo;

	//	BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	//}



	BaseShader::CreateDescriptorSets();

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		VkDescriptorImageInfo imageInfo2 = {};
		imageInfo2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo2.imageView = textureImageView2; 
		imageInfo2.sampler = textureSampler2; 

		VkDescriptorImageInfo CubeMapInfo = {};
		CubeMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		CubeMapInfo.imageView = skybox.textureImageView;
		CubeMapInfo.sampler = skybox.textureSampler;

		VkDescriptorBufferInfo FragmentBufferInfo = {};
		FragmentBufferInfo.buffer = FragmentUniformBuffers[i];
		FragmentBufferInfo.offset = 0;
		FragmentBufferInfo.range = sizeof(FragmentUniformBufferObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites = {};

		VkWriteDescriptorSet descriptor1 = {};
		descriptor1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor1.dstSet = descriptorSets[i];
		descriptor1.dstBinding = 0;
		descriptor1.dstArrayElement = 0;
		descriptor1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor1.descriptorCount = 1;
		descriptor1.pBufferInfo = &bufferInfo;
		descriptorWrites.emplace_back(descriptor1);

		VkWriteDescriptorSet descriptor2 = {};
		descriptor2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor2.dstSet = descriptorSets[i];
		descriptor2.dstBinding = 1;
		descriptor2.dstArrayElement = 0;
		descriptor2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor2.descriptorCount = 1;
		descriptor2.pImageInfo = &imageInfo;
		descriptorWrites.emplace_back(descriptor2);

		VkWriteDescriptorSet descriptor3 = {};
		descriptor3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor3.dstSet = descriptorSets[i];
		descriptor3.dstBinding = 2;
		descriptor3.dstArrayElement = 0;
		descriptor3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor3.descriptorCount = 1;
		descriptor3.pImageInfo = &imageInfo2;
		descriptorWrites.emplace_back(descriptor3);

		VkWriteDescriptorSet descriptor4 = {};
		descriptor4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor4.dstSet = descriptorSets[i];
		descriptor4.dstBinding = 3;
		descriptor4.dstArrayElement = 0;
		descriptor4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor4.descriptorCount = 1;
		descriptor4.pImageInfo = &CubeMapInfo;
		descriptorWrites.emplace_back(descriptor4);

		VkWriteDescriptorSet descriptor5 = {};
		descriptor5.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor5.dstSet = descriptorSets[i];
		descriptor5.dstBinding = 4;
		descriptor5.dstArrayElement = 0;
		descriptor5.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptor5.descriptorCount = 1;
		descriptor5.pBufferInfo = &FragmentBufferInfo;
		descriptorWrites.emplace_back(descriptor5);

		vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}
