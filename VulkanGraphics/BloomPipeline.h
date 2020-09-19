#pragma once
#include "GraphicsPipeline.h"

class BloomPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	BloomPipeline();
	BloomPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~BloomPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

