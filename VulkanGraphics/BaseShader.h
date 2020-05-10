#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "VulkanBufferManager.h"
#include "Structs.h"
class BaseShader
{
protected:
	VulkanDevice DeviceInfo;

	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);

public:
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	std::vector<VkBuffer> FragmentUniformBuffers;
	std::vector<VkDeviceMemory> FragmentUniformBuffersMemory;

	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;

	BaseShader();
	BaseShader(VulkanDevice deviceInfo);
	~BaseShader();

	void CreateShaderPipeLine();
	void CreateUniformBuffers();
	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets();
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> DescriptorList);

	VkBuffer GetUniformBuffers(int i) { return uniformBuffers[i]; }
	VkDescriptorSet GetDescriptorSet(int i) { return descriptorSets[i]; }

	void Destory();
	void DestorySwapChain();
};

