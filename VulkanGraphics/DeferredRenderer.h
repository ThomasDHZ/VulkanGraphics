#pragma once
#include "BaseRenderer.h"

class DeferredRenderer : public BaseRenderer
{
private:
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	DeferredRenderer();
	DeferredRenderer(std::vector<Mesh>* meshList, std::vector<Model>* modelList, VkInstance instance, GLFWwindow* window);
	~DeferredRenderer();

	void createCommandBuffers();
	void UpdateSwapChain();
};