#pragma once
#include "BaseRenderer.h"
#include "InputAttachment.h"

class ForwardRenderer : public BaseRenderer
{
private:

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	ForwardRenderer();
	ForwardRenderer(std::vector<Mesh>* meshList, std::vector<Model>* modelList, VkInstance instance, GLFWwindow* window);
	~ForwardRenderer();

	void createCommandBuffers();
	void UpdateSwapChain();
};

