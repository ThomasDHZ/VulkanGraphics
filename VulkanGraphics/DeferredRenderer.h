#pragma once
#include "BaseRenderer.h"

class DeferredRenderer : public BaseRenderer
{
private:
	InputAttachment PositionAttachment;
	InputAttachment NormalAttachment;
	InputAttachment AlbedoAttachment;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	DeferredRenderer();
	DeferredRenderer(std::vector<Mesh>* meshList, std::vector<Model>* modelList, SkyBox* skybox, VkInstance instance, GLFWwindow* window);
	~DeferredRenderer();

	void createCommandBuffers();
	void UpdateSwapChain();
};