#pragma once
#include "VulkanStarter.h"
#include "InputAttachment.h"
#include "VulkanSwapChain.h"
#include "FrameBuffer.h"
#include "Renderer2DPipeline.h"

class VulkanRenderer2D : public VulkanStarter
{
	friend class VulkanGraphics;
	friend class VulkanResources;
private:
	struct VulkanSemaphores
	{
		VkSemaphore ImageAcquiredSemaphore;
		VkSemaphore RenderCompleteSemaphore;

		void Destory(VkDevice device)
		{
			vkDestroySemaphore(device, RenderCompleteSemaphore, nullptr);
			vkDestroySemaphore(device, ImageAcquiredSemaphore, nullptr);

			RenderCompleteSemaphore = VK_NULL_HANDLE;
			ImageAcquiredSemaphore = VK_NULL_HANDLE;
		}
	};

	bool framebufferResized = false;

	VkRenderPass RenderPass = VK_NULL_HANDLE;
	VulkanSwapChain SwapChain;
	InputAttachment DepthAttachment;

	Renderer2DPipeline Render2DPipeline;

	VkCommandPool MainCommandPool;
	std::vector<VkCommandBuffer> MainCommandBuffer;
	VkCommandPool SecondaryCommandPool;
	std::vector<VkCommandBuffer> SecondaryCommandBuffers;

	std::vector<VulkanSemaphores> vulkanSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	void InitializeRenderPass();
	void InitializeFramebuffers();
	void InitializeCommandBuffers();
	void InitializeSyncObjects();

protected:

	//Pretty much to section off for anything that needs to be used in VUlkanGraphics.
	size_t currentFrame = 0;
	std::vector<VkCommandBuffer> RunCommandBuffers = {};
	std::vector<VkFramebuffer> SwapChainFramebuffers;

	void UpdateSwapChain(GLFWwindow* window);
	void StartFrame(GLFWwindow* window);
	void EndFrame(GLFWwindow* window);
	void DestoryVulkan();

	bool UpdateCommandBuffers = true;

public:

	VulkanRenderer2D();
	VulkanRenderer2D(GLFWwindow* window);
	~VulkanRenderer2D();

	uint32_t DrawFrame = 0;
};
