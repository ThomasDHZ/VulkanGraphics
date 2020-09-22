#pragma once
#include "GraphicsPipeline.h"

class WireFramePipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	WireFramePipeline();
	WireFramePipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~WireFramePipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

