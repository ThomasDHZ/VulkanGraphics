#pragma once
#include "BaseRenderer.h"

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}
};

const std::vector<uint32_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

class ForwardRenderer : public BaseRenderer
{
private:
	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();
	void createFramebuffers();

public:
	ForwardRenderer();
	ForwardRenderer(VkInstance instance, GLFWwindow* window);
	~ForwardRenderer();

	void createCommandBuffers(const std::vector<Mesh>& mesh);
	void UpdateSwapChain();
};

