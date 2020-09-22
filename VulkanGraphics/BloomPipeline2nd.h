#pragma once
#include "GraphicsPipeline.h"

class BloomPipeline2nd : public GraphicsPipeline
{
private:
	void CreateDescriptorSetLayout(VulkanRenderer& renderer);
	void CreateShaderPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
public:
	BloomPipeline2nd();
	BloomPipeline2nd(VulkanRenderer& renderer, const VkRenderPass& renderPass);
	~BloomPipeline2nd();

	void UpdateGraphicsPipeLine(VulkanRenderer& renderer, const VkRenderPass& renderPass);
};

