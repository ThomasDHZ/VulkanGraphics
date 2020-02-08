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

void Shader::CreateDescriptorSets(VkImageView SkyboxtextureImageView, VkSampler SkyboxtextureSampler, VkImageView textureImageView, VkSampler textureSampler, VkImageView textureImageView2, VkSampler textureSampler2)
{
	BaseShader::CreateDescriptorSets();

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = textureImageView;
	DiffuseMap.sampler = textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = textureImageView2;
	SpecularMap.sampler = textureSampler2;

	VkDescriptorImageInfo CubeMapInfo = {};
	CubeMapInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	CubeMapInfo.imageView = SkyboxtextureImageView;
	CubeMapInfo.sampler = SkyboxtextureSampler;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		VkDescriptorBufferInfo FragmentBufferInfo = {};
		FragmentBufferInfo.buffer = FragmentUniformBuffers[i];
		FragmentBufferInfo.offset = 0;
		FragmentBufferInfo.range = sizeof(FragmentUniformBufferObject);

		std::array<WriteDescriptorSetInfo, 5>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		WriteDescriptorInfo[3].DstBinding = 3;
		WriteDescriptorInfo[3].DstSet = descriptorSets[i];
		WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[3].DescriptorImageInfo = CubeMapInfo;

		WriteDescriptorInfo[4].DstBinding = 4;
		WriteDescriptorInfo[4].DstSet = descriptorSets[i];
		WriteDescriptorInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[4].DescriptorBufferInfo = FragmentBufferInfo;

		BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}
