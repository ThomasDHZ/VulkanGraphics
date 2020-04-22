#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>

class SwapChain
{
private:
	VkSurfaceFormatKHR FindSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR FindSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice);
	void SetUpSwapChain(GLFWwindow* window, const VkInstance& instance, VkDevice device, VkPhysicalDevice PhysicalDevice);
public:
	uint32_t SurfaceFormatCount;
	uint32_t PresentModeCount;
	uint32_t SwapChainImageCount;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	VkSurfaceFormatKHR SwapChainFormat;
	VkPresentModeKHR SwapChainPresentMode;

	VkSwapchainKHR Swapchain;
	VkSurfaceKHR SwapChainSurface;
	VkSurfaceCapabilitiesKHR SwapChainCapabilities;
	VkExtent2D SwapChainSize;

	std::vector<VkSurfaceFormatKHR> CompatibleSwapChainFormatList;
	std::vector<VkPresentModeKHR> CompatiblePresentModesList;
	std::vector<VkImage> SwapChainImages;

	SwapChain();
	SwapChain(GLFWwindow* window, const VkInstance& instance, VkDevice device, VkPhysicalDevice PhysicalDevice);
	~SwapChain();
};

