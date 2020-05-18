#pragma once
#include "VulkanStarter.h"
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include "InputAttachment.h"
#include "SkyBoxPipeline.h"
#include "WireFramePipeline.h"
#include "FrameBuffer.h"
#include "FrameBufferRenderingPipeline.h"
#include "DebugLightRenderingPipeline.h"

struct VulkanRendererSettings
{
	bool ShowMeshLines = false;
	bool ShowSkyBox = true;
	bool ShowDebugLightMeshs = true;

	bool operator!=(const VulkanRendererSettings& OtherSettings)
	{
		return (ShowMeshLines != OtherSettings.ShowMeshLines ||
				ShowSkyBox != OtherSettings.ShowSkyBox ||
				ShowDebugLightMeshs != OtherSettings.ShowDebugLightMeshs);
	}
};

class VulkanRenderer : public VulkanStarter
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

	InputAttachment ColorAttachment;
	InputAttachment DepthAttachment;

	VulkanSwapChain SwapChain;

	ForwardRenderingPipeline GraphicsPipeline;
	FrameBufferRenderingPipeline FrameBufferPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	WireFramePipeline MeshviewPipeline;
	SkyBoxPipeline SkyboxPipeline;

	FrameBuffer framebuffer;

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

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

	VulkanRendererSettings Settings;
	uint32_t DrawFrame = 0;
};
