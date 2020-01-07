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

class ValkanGraphics
{
private:
	const bool EnableValidationLayers;
	std::vector<const char*> ValidationLayers;

	VulkanWindow Window;
	VkInstance VulkanInstance;
	VulkanDebugger VulkanDebug;
	VkGPUInfo GPUInfo;

	void SetUpVulkanInstance();
	void SetUpDebugger();
	void SetUpPhysicalDevice();
	void SetUpLogicalDevice();

	VulkanQueueFamily FindQueueFamilies(VkPhysicalDevice physicalDevice);

	void MainLoop();
	bool ValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();

public:
	ValkanGraphics(unsigned int width, unsigned int height, const char* windowName);
	~ValkanGraphics();
	void Run();
};


