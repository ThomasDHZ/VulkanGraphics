#pragma once
#include <vulkan/vulkan.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

#include "VulkanWindow.h"
#include "VulkanDebugger.h"
#include "VulkanQueueFamily.h"
#include <optional>

struct VkGPUInfo
{
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class ValkanGraphics
{
private:
	const bool EnableValidationLayers;
	std::vector<const char*> ValidationLayers;
	std::vector<const char*> DeviceExtensions;


	VulkanWindow Window;
	VkInstance VulkanInstance;
	VulkanDebugger VulkanDebug;
	VkGPUInfo GPUInfo;
	VkQueue GraphicsQueue;
	VkQueue PresentQueue;
	VkSurfaceKHR VulkanSurface;
	VkSwapchainKHR SwapChain;

	std::vector<VkImage> SwapChainImages;
	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;

	void SetUpVulkanInstance();
	void SetUpDebugger();
	void SetUpPhysicalDevice();
	void SetUpLogicalDevice();
	void SetUpSurface();
	void SetUpSwapChain();

	VulkanQueueFamily FindQueueFamilies(VkPhysicalDevice physicalDevice);

	void MainLoop();

	bool ValidationLayerSupport();
	bool IsDeviceCompatable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR SetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	VkPresentModeKHR SetSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
	VkExtent2D SetSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	std::vector<const char*> GetRequiredExtensions();

public:
	ValkanGraphics(unsigned int width, unsigned int height, const char* windowName);
	~ValkanGraphics();
	void Run();
};


