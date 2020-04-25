#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanRenderer
{
private:
	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);
	void InitializeVulkan(GLFWwindow* window);
public:

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;

	VulkanDebugger VulkanDebug;
	VulkanSwapChain swapChain;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	void UpdateSwapChain(GLFWwindow* window);

	VkSwapchainKHR GetSwapChain() { return swapChain.GetSwapChain(); }
	std::vector<VkImage> GetSwapChainImages() { return swapChain.GetSwapChainImages(); }
	std::vector<VkImageView> GetSwapChainImageViews() { return swapChain.GetSwapChainImageViews(); }
	VkExtent2D GetSwapChainResolution() { return swapChain.GetSwapChainResolution(); }
	VkSurfaceFormatKHR GetSwapChainImageFormat() { return swapChain.GetSwapChainImageFormat(); }
	uint32_t GetSwapChainImageCount() { return swapChain.GetSwapChainImageCount(); }
};

