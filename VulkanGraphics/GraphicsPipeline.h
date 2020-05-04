#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <string>
#include "Structs.h"

enum PipeLineType
{
	Pipeline_FowardRenderer,
	Pipeline_MeshView,
	Pipeline_SkyBox
};

class GraphicsPipeline
{
protected:
	VkDevice Device;
	PipeLineType PipelineType;

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
	GraphicsPipeline(VkDevice device, PipeLineType Pipelinetype);
	~GraphicsPipeline();

	void Destroy();
};

