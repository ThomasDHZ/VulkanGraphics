#pragma once
#include "GraphicsPipeline.h"

class SkyBoxPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	SkyBoxPipeline();
	SkyBoxPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~SkyBoxPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};