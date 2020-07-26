#pragma once
#include "GraphicsPipeline.h"
class DebugLightRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	DebugLightRenderingPipeline();
	DebugLightRenderingPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~DebugLightRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};
