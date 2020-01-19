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
#include <glm/glm.hpp>
#include <array>
#include "UniformBufferObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"

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
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

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
	VkPipeline LightGraphicsPipeline;
	VkCommandPool CommandPool;
	VkSemaphore ImageAvailableSemaphore;
	VkSemaphore RenderFinishedSemaphore;
	VkImage DepthImage;
	VkDeviceMemory DepthImageMemory;
	VkImageView DepthImageView;

	VkDescriptorSetLayout DescriptorSetLayout;

	Mesh Mesh1;
	Mesh Mesh2;

	UniformBufferObject<UniformBufferObject2> UniformBufferobject;
	UniformBufferObject<LightingStruct> LightBufferStuff;

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
	void SetDescriptorSetLayout();
	void SetUpGraphicsPipeLine();
	void SetUpFrameBuffers();
	void SetUpCommandPool();
	void SetUpDepthBuffer();
	void SetUpVertexBuffers();
	void SetUpUniformBuffers();
	void SetUpDescriptorPool();
	void SetUpDescriptorSets();
	void SetUpCommandBuffers();
	void SetUpSyncObjects();
	void CleanUpSwapChain();
	void RecreateSwapChain();
	void DrawFrame();

	void CreatePipeLine();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VulkanQueueFamily FindQueueFamilies(VkPhysicalDevice physicalDevice);
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	void MainLoop();

	bool ValidationLayerSupport();
	bool IsDeviceCompatable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR SetSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& AvailableFormats);
	VkPresentModeKHR SetSwapPresentMode(const std::vector<VkPresentModeKHR>& AvailablePresentModes);
	VkExtent2D SetSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	std::vector<const char*> GetRequiredExtensions();


	void UpdateMouse();
	void UpdateKeyboard();
public:
	ValkanGraphics(unsigned int width, unsigned int height, const char* windowName);
	~ValkanGraphics();
	void Run();
};


