#pragma once
#include "BaseMesh.h"

class Screen2DMesh : public BaseMesh
{
private:

	void CreateDescriptorPool();
	void CreateDescriptorSets();

public:
	Screen2DMesh();
	Screen2DMesh(VulkanDevice deviceInfo);
	~Screen2DMesh();

	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void UpdateSwapChain();
};


