#pragma once
#include "BaseRenderer.h"
#include "Screen2DMesh.h"

class Renderer2D : public BaseRenderer
{

	std::array<Texture2D, 3> DisplayTexture;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	Renderer2D();
	Renderer2D(VkInstance instance, GLFWwindow* window);
	~Renderer2D();

	Screen2DMesh Display2D;

	void createCommandBuffers();
	void UpdateFrame(size_t currentFrame);
	void UpdateSwapChain();
	void ClearSwapChain();
	void Destory();
};

