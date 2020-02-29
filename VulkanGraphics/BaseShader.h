#pragma once
#include "Structs.h"
#include <array>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>

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

	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;


	BaseShader();
	BaseShader(VulkanDevice deviceInfo);
	~BaseShader();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	void Destory();
	void DestorySwapChain();
};

