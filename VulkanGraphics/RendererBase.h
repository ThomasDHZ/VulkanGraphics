#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"

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
	bool ShowDebugLightMesh = true;
	bool ShowDebugCollisionMesh = false;
	bool TwoDMode = false;

	bool operator!=(const VulkanRendererSettings& OtherSettings)
	{
		return (ShowMeshLines != OtherSettings.ShowMeshLines ||
			ShowSkyBox != OtherSettings.ShowSkyBox ||
			ShowDebugLightMesh != OtherSettings.ShowDebugLightMesh ||
			ShowDebugCollisionMesh != OtherSettings.ShowDebugCollisionMesh ||
			TwoDMode != OtherSettings.TwoDMode);
	}
};
class RendererBase
{
private:
protected:

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

	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;

	VulkanDebugger VulkanDebug;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	VkCommandPool MainCommandPool;
	std::vector<VkCommandBuffer> MainCommandBuffer;
	VkCommandPool SecondaryCommandPool;
	std::vector<VkCommandBuffer> SecondaryCommandBuffers;

	std::vector<VulkanSemaphores> vulkanSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::vector<VkFramebuffer> OffscreenSwapChainFramebuffers;

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);


	VulkanSwapChain SwapChain;
	void InitializeCommandBuffers();
	void InitializeSyncObjects();

	void Destory();
public:
	RendererBase();
	RendererBase(GLFWwindow* window);
	~RendererBase();
};

