#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <string>
#include "Structs.h"

class GraphicsPipeline
{
protected:
	VulkanDevice DeviceInfo;

	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

	void CreateDescriptorSetLayout(std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
	void CreatePipeLineLayout(VkPipelineLayoutCreateInfo PipelineLayoutInfo);
	void CreatePipeLine(VkGraphicsPipelineCreateInfo PipeLineInfo);

public:
	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;
	VkDescriptorSetLayout ShaderPipelineDescriptorLayout;

	GraphicsPipeline();
	GraphicsPipeline(VulkanDevice deviceInfo);
	~GraphicsPipeline();

	virtual void ClearSwapChain();
};

