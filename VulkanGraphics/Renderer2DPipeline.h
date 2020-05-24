#pragma once
#include "GraphicsPipeline.h"

class Renderer2DPipeline : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout();
	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
public:
	Renderer2DPipeline();
	Renderer2DPipeline(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
	~Renderer2DPipeline();

	void UpdateGraphicsPipeLine(VkExtent2D swapChainExtent, VkRenderPass& renderPass, VkDevice device);
};

