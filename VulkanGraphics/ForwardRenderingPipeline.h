#pragma once
#include "GraphicsPipeline.h"

class ForwardRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
public:
	ForwardRenderingPipeline();
	ForwardRenderingPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
	~ForwardRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass, const VkPipelineColorBlendStateCreateInfo& ColorBlendingSettings, const RendererType rendererType);
};

