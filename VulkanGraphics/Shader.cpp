#include "Shader.h"
#include "Model.h"

Shader::Shader() : BaseShader()
{
}

Shader::Shader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout layout, ShaderTextureInputs ShaderInput) : BaseShader(deviceInfo)
{
	RecreateSwapChainInfo(swapChainExtent, renderPass, layout, ShaderInput);
}

Shader::~Shader()
{
}

void Shader::RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout layout, ShaderTextureInputs ShaderInput)
{
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(layout, ShaderInput);
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
}

void Shader::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 3>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseShader::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Shader::CreateDescriptorSets(VkDescriptorSetLayout layout, ShaderTextureInputs TextureInfo)
{
	BaseShader::CreateDescriptorSets(layout);

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureInfo.textureImageView;
	DiffuseMap.sampler = TextureInfo.textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureInfo.textureImageView2;
	SpecularMap.sampler = TextureInfo.textureSampler2;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject2);

		std::array<WriteDescriptorSetInfo, 3>  WriteDescriptorInfo = {};

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

		BaseShader::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}
