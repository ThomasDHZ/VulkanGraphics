#pragma once
#include "GraphicsPipeline.h"

class FrameBufferRenderingPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	FrameBufferRenderingPipeline();
	FrameBufferRenderingPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~FrameBufferRenderingPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

