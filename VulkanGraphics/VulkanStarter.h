#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanStarter
{
private:
protected:

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

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);

	void Destory();
public:
	VulkanStarter();
	VulkanStarter(GLFWwindow* window);
	~VulkanStarter();
};

