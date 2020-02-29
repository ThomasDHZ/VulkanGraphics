#pragma once
#include "SkyBox.h"

struct DescriptorSetLayoutBindingInfo
{
	uint32_t Binding;
	VkDescriptorType DescriptorType;
	VkShaderStageFlags StageFlags;
};

struct DescriptorPoolSizeInfo
{
	VkDescriptorType DescriptorType;
};

struct WriteDescriptorSetInfo
{
	uint32_t DstBinding;
	VkDescriptorSet DstSet;
	VkDescriptorType DescriptorType;
	VkDescriptorBufferInfo DescriptorBufferInfo;
	VkDescriptorImageInfo DescriptorImageInfo;
};

class BaseShader
{
protected:
	VulkanDevice DeviceInfo;

	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	void CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
	void CreateShaderPipeLine();
	void CreateUniformBuffers();
	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VkDescriptorSetLayout layout);
	void CreateDescriptorSets();
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> DescriptorList);

public:
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	std::vector<VkBuffer> LightFragmentUniformBuffers;
	std::vector<VkDeviceMemory> LightFragmentUniformBuffersMemory;

	std::vector<VkBuffer> DebugUniformBuffers;
	std::vector<VkDeviceMemory> DebugBuffersMemory;

	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;

	BaseShader();
	BaseShader(VulkanDevice deviceInfo);
	~BaseShader();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	void Destory();
	void DestorySwapChain();
};

