#pragma once
#include "GraphicsPipeline.h"

class CollisionDebugPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanEngine& renderer);
	void CreateShaderPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
public:
	CollisionDebugPipeline();
	CollisionDebugPipeline(VulkanEngine& renderer, const VkRenderPass& renderPass);
	~CollisionDebugPipeline();

	void UpdateGraphicsPipeLine(VulkanEngine& renderer, const VkRenderPass& renderPass);
};

