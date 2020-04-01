#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>
#include "Structs.h"
#include <GLFW\glfw3.h>
#include <optional>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include "Vertex.h"
#include "Mesh.h"
#include "Model.h"
#include "InputAttachment.h"
#include "SkyBox.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class BaseRenderer
{
private:
protected:
	GLFWwindow* Window;

	std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	VkFormat findDepthFormat();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void createSurface(VkInstance instance);
	void pickPhysicalDevice(VkInstance instance);
	void createLogicalDevice();
	void createSwapChain();
	void createImageViews();
	void createRenderPass(VkSubpassDescription subpass, VkSubpassDependency dependency, std::vector<VkAttachmentDescription> attachments);
	void createDescriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);
	void createFramebuffers(std::vector<VkImageView> attachments, int Image);
	void createCommandPool();
	void createSkyboxDesriptorSetLayout(std::vector<VkDescriptorSetLayoutBinding> bindings);

public:
	BaseRenderer();
	BaseRenderer(std::vector<Mesh>* meshList, std::vector<Model>* modelList, SkyBox* skybox, VkInstance instance, GLFWwindow* window);
	~BaseRenderer();

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkDescriptorSetLayout skyboxdescriptorSetLayout;
	VkPipelineLayout skyboxpipelineLayout;
	VkPipeline skyboxgraphicsPipeline;

	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

	std::vector<VkCommandBuffer> commandBuffers;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkDevice device;
	VkCommandPool commandPool;

	InputAttachment DepthAttachment;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	std::vector<Mesh>* MeshList;
	std::vector<Model>* ModelList;
	SkyBox* skyBox;
	SkyBoxPipeline skyBoxPipeline;

	void createCommandBuffers(std::vector<VkClearValue> clearValues);
	void createSyncObjects();
	void ClearSwapChain();
	void Destory();
	VulkanDevice UpdateDeviceInfo();
};

