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

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};
		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(Vertex);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		
		return BindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> AttributeDescriptions = {};

		AttributeDescriptions[0].binding = 0;
		AttributeDescriptions[0].location = 0;
		AttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[0].offset = offsetof(Vertex, pos);

		AttributeDescriptions[1].binding = 0;
		AttributeDescriptions[1].location = 1;
		AttributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescriptions[1].offset = offsetof(Vertex, color);

		AttributeDescriptions[2].binding = 0;
		AttributeDescriptions[2].location = 2;
		AttributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return AttributeDescriptions;
	}
};

const std::vector<Vertex> vertices = {
	{{-1.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, -1.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, 1.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
};

const std::vector<Vertex> vertices2 = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
};

const std::vector<uint16_t> indices =
{
	0, 1, 2, 2, 3, 0
};

const std::vector<uint16_t> indices2 = 
{
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

struct UniformBufferObject2 {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct LightingStruct
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
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
	VkDescriptorPool DescriptorPool;
	VkImage TextureImage;
	VkDeviceMemory TextureImageMemory;
	VkImageView TextureImageView;
	VkSampler TextureSampler;

	VertexBufferObject<Vertex> VertexBuffer;
	VertexBufferObject<Vertex> VertexBuffer2;
	IndexBufferObject IndexBuffer;
	IndexBufferObject IndexBuffer2;
	UniformBufferObject<UniformBufferObject2> UniformBufferobject;
	UniformBufferObject<UniformBufferObject2> UniformBufferobject2;
	UniformBufferObject<LightingStruct> LightBufferStuff;

	std::vector<VkDescriptorSet> DescriptorSets;
	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkCommandBuffer> CommandBuffers2;
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
	void SetUpTextureImage();
	void SetUpTextureImageView();
	void SetUpTextureSampler();
	void SetUpVertexBuffers();
	void SetUpIndexBuffers();
	void SetUpUniformBuffers();
	void SetUpDescriptorPool();
	void SetUpDescriptorSets();
	void SetUpCommandBuffers();
	void SetUpCommandBuffers2();
	void SetUpSyncObjects();
	void CleanUpSwapChain();
	void RecreateSwapChain();
	void DrawFrame();

	void CreatePipeLine();
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
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


