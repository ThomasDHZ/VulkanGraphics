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
#include "ShaderCompiler.h"
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
	const int MAX_FRAMES_IN_FLIGHT = 2;

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
	ShaderCompiler CompileShader;
	VkRenderPass RenderPass;
	VkPipelineLayout PipelineLayout;
	VkPipeline GraphicsPipeline;
	VkCommandPool CommandPool;
	VkSemaphore ImageAvailableSemaphore;
	VkSemaphore RenderFinishedSemaphore;

	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkSemaphore> ImageAvailableSemaphores;
	std::vector<VkSemaphore> RenderFinishedSemaphores;
	std::vector<VkFence> InFlightFences;
	std::vector<VkFence> ImagesInFlight;

	VkFormat SwapChainImageFormat;
	VkExtent2D SwapChainExtent;
	size_t CurrentFrame = 0;

	void SetUpVulkanInstance();
	void SetUpDebugger();
	void SetUpPhysicalDevice();
	void SetUpLogicalDevice();
	void SetUpSurface();
	void SetUpSwapChain();
	void SetUpImageViews();
	void SetUpRenderPass();
	void SetUpGraphicsPipeLine();
	void SetUpFrameBuffers();
	void SetUpCommandPool();
	void SetUpCommandBuffers();
	void SetUpSyncObjects();

	void CleanUpSwapChain();
	void RecreateSwapChain();
	void DrawFrame();

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


