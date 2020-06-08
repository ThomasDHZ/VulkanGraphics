#pragma once
#include "GraphicsPipeline.h"

class CollisionDebugPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	CollisionDebugPipeline();
	CollisionDebugPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~CollisionDebugPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

