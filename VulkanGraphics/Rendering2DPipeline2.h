#pragma once
#include "GraphicsPipeline.h"

class Rendering2DPipeline2 : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	Rendering2DPipeline2();
	Rendering2DPipeline2(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~Rendering2DPipeline2();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

