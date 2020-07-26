#pragma once
#include "GraphicsPipeline.h"

class ForwardRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	ForwardRenderingPipeline();
	ForwardRenderingPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~ForwardRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

