#pragma once
#include "GraphicsPipeline.h"

class Rendering2DPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
public:
	Rendering2DPipeline();
	Rendering2DPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
	~Rendering2DPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
};

