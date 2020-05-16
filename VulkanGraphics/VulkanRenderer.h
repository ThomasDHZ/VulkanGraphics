#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include <array>
#include "InputAttachment.h"
#include "SkyBoxPipeline.h"
#include "WireFramePipeline.h"
#include "FrameBuffer.h"
#include "FrameBufferRenderingPipeline.h"
#include "DebugLightRenderingPipeline.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int MAX_FRAMES_IN_FLIGHT = 2;

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

class VulkanRenderer
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

	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkRenderPass RenderPass = VK_NULL_HANDLE;

	InputAttachment ColorAttachment;
	InputAttachment DepthAttachment;

	VulkanDebugger VulkanDebug;
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

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);
	void InitializeVulkan(GLFWwindow* window);
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
	uint32_t StartFrame(GLFWwindow* window);
	void EndFrame(GLFWwindow* window, uint32_t imageIndex);
	void DestoryVulkan();

	bool UpdateCommandBuffers = true;

public:

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

	VulkanRendererSettings Settings;
};
