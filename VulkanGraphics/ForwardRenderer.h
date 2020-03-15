#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include "Structs.h"
#include "InputAttachment.h"
#include <GLFW\glfw3.h>
#include "SwapChain.h"
#include "Vertex.h"


class ForwardRenderer
{
private:

	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void SetUpRenderPass();
public:
	SwapChain swapChain;
	VkRenderPass renderPass;

	ForwardRenderer();
	ForwardRenderer(VkInstance instance, GLFWwindow* window);
	~ForwardRenderer();

	void UpdateSwapChain();
	void Destory();
};

