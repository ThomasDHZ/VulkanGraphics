#pragma once
#include "GraphicsPipeline.h"

class MeshViewPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	MeshViewPipeline();
	MeshViewPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~MeshViewPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

