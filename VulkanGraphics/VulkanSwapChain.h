#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

class VulkanSwapChain
{
private:
	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);
	VkSurfaceFormatKHR FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

	void VulkanStart(GLFWwindow* window);
	void SetUpSwapChain(GLFWwindow* window);
	void SetUpSwapChainImageViews();
public:
	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;

	VulkanDebugger VulkanDebug;

	uint32_t SurfaceFormatCount;
	uint32_t PresentModeCount;
	uint32_t SwapChainImageCount;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	VkSurfaceFormatKHR SwapChainFormat;
	VkPresentModeKHR SwapChainPresentMode;

	VkSwapchainKHR Swapchain;
	VkSurfaceCapabilitiesKHR SwapChainCapabilities;
	VkExtent2D SwapChainSize;

	std::vector<VkExtensionProperties> SupportedExtensions;
	std::vector<VkLayerProperties> VulkanLayers;
	std::vector<VkSurfaceFormatKHR> CompatibleSwapChainFormatList;
	std::vector<VkPresentModeKHR> CompatiblePresentModesList;

	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;

	VulkanSwapChain();
	VulkanSwapChain(GLFWwindow* window);
	~VulkanSwapChain();

	void DestroyVulkan();
};

