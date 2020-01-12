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
	glm::vec2 pos;
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
		AttributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
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
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = 
{
	0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
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
	VkBuffer VertexBuffer;
	VkDeviceMemory VertexBufferMemory;
	VkBuffer IndexBuffer;
	VkDeviceMemory IndexBufferMemory;
	VkDescriptorSetLayout DescriptorSetLayout;
	VkDescriptorPool DescriptorPool;
	VkImage TextureImage;
	VkDeviceMemory TextureImageMemory;
	VkImageView TextureImageView;
	VkSampler TextureSampler;

	std::vector<VkDescriptorSet> DescriptorSets;
	std::vector<VkImage> SwapChainImages;
	std::vector<VkImageView> SwapChainImageViews;
	std::vector<VkFramebuffer> SwapChainFramebuffers;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
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
	void SetUpTextureImage();
	void SetUpTextureImageView();
	void SetUpTextureSampler();
	void SetUpVertexBuffers();
	void SetUpIndexBuffers();
	void SetUpUniformBuffer();
	void SetUpDescriptorPool();
	void SetUpDescriptorSets();
	void SetUpCommandBuffers();
	void SetUpSyncObjects();

	void CleanUpSwapChain();
	void RecreateSwapChain();
	void DrawFrame();

	VkImageView CreateImageView(VkImage image, VkFormat format);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void CopyBuffer(VkBuffer SrcBuffer, VkBuffer DstBuffer, VkDeviceSize Size);
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	VulkanQueueFamily FindQueueFamilies(VkPhysicalDevice physicalDevice);

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

public:
	ValkanGraphics(unsigned int width, unsigned int height, const char* windowName);
	~ValkanGraphics();
	void Run();
};


