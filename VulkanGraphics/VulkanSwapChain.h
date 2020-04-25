#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"

class VulkanSwapChain
{
private:
	uint32_t SurfaceFormatCount;
	uint32_t PresentModeCount;
	uint32_t SwapChainImageCount;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	VkSurfaceFormatKHR SwapChainImageFormat;
	VkPresentModeKHR SwapChainPresentMode;

	VkSurfaceCapabilitiesKHR SwapChainCapabilities;

	std::vector<VkExtensionProperties> SupportedExtensions;
	std::vector<VkLayerProperties> VulkanLayers;
	std::vector<VkSurfaceFormatKHR> CompatibleSwapChainFormatList;
	std::vector<VkPresentModeKHR> CompatiblePresentModesList;

	VkSwapchainKHR Swapchain;
	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
	VkExtent2D SwapChainResolution;

	VkSurfaceFormatKHR FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

public:

	VulkanSwapChain();
	VulkanSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	~VulkanSwapChain();

	void SetUpSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void SetUpSwapChainImageViews(VkDevice device);
	void DestroyVulkan();

	void UpdateSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	VkSwapchainKHR GetSwapChain() { return Swapchain; }
	std::vector<VkImage> GetSwapChainImages() { return SwapChainImages; }
	std::vector<VkImageView> GetSwapChainImageViews() { return SwapChainImageViews; }
	VkExtent2D GetSwapChainResolution() { return SwapChainResolution; }
	VkSurfaceFormatKHR GetSwapChainImageFormat() { return SwapChainImageFormat; }
	uint32_t GetSwapChainImageCount() { return SwapChainImageCount; }
};

