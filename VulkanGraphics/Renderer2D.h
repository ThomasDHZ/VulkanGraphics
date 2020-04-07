#pragma once
#include "BaseRenderer.h"
#include "Screen2DMesh.h"

class Renderer2D : public BaseRenderer
{
	Screen2DMesh Display2D;
	std::array<Texture2D, 3> DisplayTexture;
	size_t currentFrame = 0;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();
	void createCommandBuffers();

public:
	Renderer2D();
	Renderer2D(VkInstance instance, GLFWwindow* window);
	~Renderer2D();

	VkCommandBuffer commandBuffer2D;
	VkCommandBuffer DrawBuffer2D;
	VkCommandPool SecondCommandPool;

	void createSyncObjects();
	void UpdateSecondaryCommandBuffer(VkCommandBufferInheritanceInfo inheritanceInfo);
	void UpdateCommandBuffers();
	void UpdateFrame();
	void UpdateSwapChain();
	void ClearSwapChain();
	void Destory();
};

