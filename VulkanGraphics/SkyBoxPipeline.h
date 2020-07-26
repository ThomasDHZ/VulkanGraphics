#pragma once
#include "GraphicsPipeline.h"

class SkyBoxPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	SkyBoxPipeline();
	SkyBoxPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~SkyBoxPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};