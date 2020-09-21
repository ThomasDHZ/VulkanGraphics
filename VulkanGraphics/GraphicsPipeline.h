#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <string>
#include "Structs.h"
#include "VulkanRenderer.h"

enum RendererType
{
	RT_ForwardRenderer,
	RT_SceneRenderer,
	RT_TextureRenderer,
	RT_ShadowRenderer
};

class GraphicsPipeline
{
protected:
	std::vector<char> ReadShaderFile(const std::string& filename);
	VkShaderModule CreateShaderModule(VulkanRenderer& renderer, const std::vector<char>& code);

	void CreateDescriptorSetLayout(VulkanRenderer& renderer, std::vector<DescriptorSetLayoutBindingInfo> LayoutBindingInfo);
	void CreatePipeLineLayout(VulkanRenderer& renderer, VkPipelineLayoutCreateInfo PipelineLayoutInfo);
	void CreatePipeLine(VulkanRenderer& renderer, VkGraphicsPipelineCreateInfo PipeLineInfo);

public:
	VkPipelineLayout ShaderPipelineLayout;
	VkPipeline ShaderPipeline;
	VkDescriptorSetLayout ShaderPipelineDescriptorLayout;

	GraphicsPipeline();
	GraphicsPipeline(VulkanRenderer& renderer);
	~GraphicsPipeline();

	void UpdateSwapChain(VulkanRenderer& renderer);
	void Destroy(VulkanRenderer& renderer);
};

