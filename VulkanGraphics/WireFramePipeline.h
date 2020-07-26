#pragma once
#include "GraphicsPipeline.h"

class WireFramePipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	WireFramePipeline();
	WireFramePipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~WireFramePipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

