#pragma once
#include "GraphicsPipeline.h"

class CollisionDebugPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	CollisionDebugPipeline();
	CollisionDebugPipeline(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~CollisionDebugPipeline();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

