#pragma once
#include "BaseRenderer.h"
#include "Screen2DMesh.h"
#include "Canvas2D.h"

class Renderer2D : public BaseRenderer
{
	Texture2D MapTexture;
	Canvas2D canvas;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	Renderer2D();
	Renderer2D(VkInstance instance, GLFWwindow* window);
	~Renderer2D();

	void createCommandBuffers();
	void DrawFrame(size_t currentFrame);
	void UpdateFrame(size_t currentFrame);
	void UpdateSwapChain(size_t currentFrame);
	void ClearSwapChain();
	void Destory();
};

