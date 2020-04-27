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

struct VulkanRendererInfo
{
	VkInstance Instance = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkQueue GraphicsQueue = VK_NULL_HANDLE;
	VkQueue PresentQueue = VK_NULL_HANDLE;
	VkRenderPass RenderPass = VK_NULL_HANDLE;
	VkDescriptorSetLayout DescriptorSetLayout = VK_NULL_HANDLE;
	VkCommandPool SubCommandPool = VK_NULL_HANDLE;
	VkPipeline ShaderPipeline = VK_NULL_HANDLE;
	VkPipelineLayout ShaderPipelineLayout = VK_NULL_HANDLE;
	VkExtent2D SwapChainResolution = VkExtent2D();
	uint32_t SwapChainImageCount = 0;
};

class VulkanRenderer
{
	friend class VulkanResources;
private:
	size_t currentFrame = 0;
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

	VkCommandPool MainCommandPool;
	VkCommandPool SubCommandPool;

	std::vector<VkCommandBuffer> MainCommandBuffer;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	int GraphicsFamily = -1;
	int PresentFamily = -1;

	std::vector<VkLayerProperties> VulkanLayers;

	VulkanRendererInfo RendererInfo;
	std::vector<Mesh>* MeshList;


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
	void UpdateRendererInfo();
	
public:

	VulkanRenderer();
	VulkanRenderer(GLFWwindow* window, std::vector<Mesh>* meshList);
	~VulkanRenderer();

	GUIDebugger guiDebugger;
	std::vector<VkCommandBuffer> SubCommandBuffers;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	void UpdateCommandBuffers();
	void UpdateSwapChain(GLFWwindow* window);
	void Update(uint32_t currentImage);
	void Draw(GLFWwindow* window);
	void DestoryVulkan();

	VulkanRendererInfo* GetRendererInfo() { return &RendererInfo; };
};