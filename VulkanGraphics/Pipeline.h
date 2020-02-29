#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <string>
#include "Structs.h"

class GraphicsPipeline
{
protected:
	VulkanDevice DeviceInfo;

	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;
	VkDescriptorSetLayout ShaderPipelineDescriptorLayout;

	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	void CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);

public:
	GraphicsPipeline();
	GraphicsPipeline(VulkanDevice deviceInfo);
	~GraphicsPipeline();

	void Destory();
	void DestorySwapChain();
};

