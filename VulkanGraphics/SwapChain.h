#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <optional>
#include "Structs.h"
#include <GLFW\glfw3.h>

const std::vector<const char*> validationLayers = 
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = 
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain
{
private:
	VkInstance Instance;
	GLFWwindow* Window;

	bool enableValidationLayers = true;

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	void SetUpSurface();
	void SetPhysicalDevice();
	void CreateLogicalDevice();


public:
	VulkanDevice DeviceInfo;

	VkSurfaceKHR surface;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	SwapChain();
	SwapChain(VkInstance instance, GLFWwindow* window);
	~SwapChain();

	void SetUpSwapChain();
	void SetUpSwapChainImageViews();

	void UpdateSwapChain();
	void DestorySwapChain();
};

