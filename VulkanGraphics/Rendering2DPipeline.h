#pragma once
#include "GraphicsPipeline.h"

class Rendering2DPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	Rendering2DPipeline();
	Rendering2DPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~Rendering2DPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

