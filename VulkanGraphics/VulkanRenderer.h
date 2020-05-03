#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include <array>
#include "InputAttachment.h"
#include "SkyBoxPipeline.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanRenderer
{
	friend class VulkanGraphics;
	friend class VulkanResources;
private:
	bool framebufferResized = false;

	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkRenderPass RenderPass = VK_NULL_HANDLE;

	InputAttachment DepthAttachment;

	VulkanDebugger VulkanDebug;
	VulkanSwapChain swapChain;
	ForwardRenderingPipeline GraphicsPipeline;
	SkyBoxPipeline SkyboxPipeline;

	std::vector<VkCommandBuffer> MainCommandBuffer;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();

	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice GPUDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice GPUDevice);

	void FindQueueFamilies(VkPhysicalDevice PhysicalDevice, VkSurfaceKHR Surface);
	void InitializeVulkan(GLFWwindow* window);
	void InitializeRenderPass();
	void InitializeFramebuffers();
	void InitializeCommandBuffers();
	void InitializeSyncObjects();

protected:

	size_t currentFrame = 0;
	std::vector<VkCommandBuffer> RunCommandBuffers = {};

	bool UpdateCommandBuffers = true;

	void UpdateSwapChain(GLFWwindow* window);
	uint32_t StartFrame(GLFWwindow* window);
	void EndFrame(GLFWwindow* window, uint32_t imageIndex);
	void DestoryVulkan();

public:

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

	VkCommandPool SubCommandPool;
	std::vector<VkCommandBuffer> SubCommandBuffers;
	VkCommandPool MainCommandPool;
	std::vector<VkFramebuffer> swapChainFramebuffers;
};
