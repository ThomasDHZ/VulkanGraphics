#pragma once
#include "BaseRenderer.h"

class Renderer2D : public BaseRenderer
{
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	Renderer2D();
	Renderer2D(VkInstance instance, GLFWwindow* window);
	~Renderer2D();

	void createCommandBuffers();
	void UpdateSwapChain();
};

