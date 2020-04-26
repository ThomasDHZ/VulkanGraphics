#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <GLFW\glfw3.h>
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include <array>
#include "InputAttachment.h"
#include "Mesh.h"
#include "GUIDebugger.h"

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanRenderer
{
private:
	size_t currentFrame = 0;
	bool framebufferResized = false;

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
	void InitializeGUIDebugger(GLFWwindow* window);

public:

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();

	GUIDebugger guiDebugger;
	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkRenderPass RenderPass = VK_NULL_HANDLE; 
	std::vector<VkFramebuffer> swapChainFramebuffers;

	InputAttachment DepthAttachment;

	VulkanDebugger VulkanDebug;
	VulkanSwapChain swapChain;
	ForwardRenderingPipeline GraphicsPipeline;

	VkCommandPool MainCommandPool;
	VkCommandPool commandPool;

	std::vector<VkCommandBuffer> MainCommandBuffer;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	void UpdateSwapChain(GLFWwindow* window, Mesh mesh);
	void Update(uint32_t currentImage, Mesh mesh);
	void Draw(GLFWwindow* window, Mesh mesh);
	void DestoryVulkan();

	VkSwapchainKHR GetSwapChain() { return swapChain.GetSwapChain(); }
	std::vector<VkImage> GetSwapChainImages() { return swapChain.GetSwapChainImages(); }
	std::vector<VkImageView> GetSwapChainImageViews() { return swapChain.GetSwapChainImageViews(); }
	VkExtent2D GetSwapChainResolution() { return swapChain.GetSwapChainResolution(); }
	VkSurfaceFormatKHR GetSwapChainImageFormat() { return swapChain.GetSwapChainImageFormat(); }
	uint32_t GetSwapChainImageCount() { return swapChain.GetSwapChainImageCount(); }
};

