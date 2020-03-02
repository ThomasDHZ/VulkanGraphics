#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

#include "SkyBox.h"
#include "camera.h"
#include "Model.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "InputAttachment.h"
#include "FrameBuffer.h"
#include "VulkanDebugger.h"
#include "VulkanWindow.h"

const int WIDTH = 1800;
const int HEIGHT = 1600;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

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

const std::vector<Vertex> vertices2 =
{
	{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  0.0f}},
	{{0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  1.0f,  1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {  0.0f,  0.0f, -1.0f}, { 0.0f,  0.0f}},

	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  1.0f,  1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {  0.0f,  0.0f,  1.0f}, {  0.0f,  0.0f}},

	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f, -0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{-0.5f, -0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, { -1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},

	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  0.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  1.0f,  0.0f,  0.0f}, {  1.0f,  0.0f}},

	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {  0.0f, -1.0f,  0.0f}, {  0.0f,  1.0f}},

	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}},
	{{0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  1.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  1.0f,  0.0f}},
	{{-0.5f,  0.5f,  0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {  0.0f,  1.0f,  0.0f}, {  0.0f,  1.0f}}
};

const std::vector<uint16_t> indices = {};

glm::vec3 cubePositions[] = 
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

class HelloTriangleApplication {
public:
	void run() 
	{
		Window = VulkanWindow(WIDTH, HEIGHT, "Vulkan");
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	int DebugLayer = 0;
	VulkanDebugger VulkanDebug;
	VulkanWindow Window;

	VulkanDevice DeviceInfo;

	MainPipeline mainPipeline;
	FramebufferPipeline frameBufferPipeline;

	Camera camera;
	//SkyBox Skybox;
	Model MeshList;
	Model Nanosuit;
	//SkyBoxShader skyBoxShader;
	Texture2D texture;
	Texture2D texture2;
	CubeMapTexture cubeMapTexture;

	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	VkInstance instance;
	VkSurfaceKHR surface;

	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

	FrameBuffer frameBuffer;
	InputAttachment PositionAttachment;
	InputAttachment NormalAttachment;
	InputAttachment AlbedoAttachment;
	InputAttachment DepthAttachment;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	bool framebufferResized = false;

	void initVulkan() {
		createInstance();
		SetUpDebugger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createCommandPool();

		PositionAttachment = InputAttachment(DeviceInfo, AttachmentType::VkPositionAttachment, swapChainExtent.width, swapChainExtent.height);
		NormalAttachment = InputAttachment(DeviceInfo, AttachmentType::VkNormalAttachment, swapChainExtent.width, swapChainExtent.height);
		AlbedoAttachment = InputAttachment(DeviceInfo, AttachmentType::VkAlbedoAttachment, swapChainExtent.width, swapChainExtent.height);
		DepthAttachment = InputAttachment(DeviceInfo, AttachmentType::VkDepthAttachemnt, swapChainExtent.width, swapChainExtent.height);

		mainPipeline = MainPipeline(swapChainExtent, renderPass, DeviceInfo);
		frameBufferPipeline = FramebufferPipeline(swapChainExtent, renderPass, DeviceInfo);

		CubeMapLayout layout;
		layout.Left = "texture/skybox/left.jpg";
		layout.Right = "texture/skybox/right.jpg";
		layout.Top = "texture/skybox/top.jpg";
		layout.Bottom = "texture/skybox/bottom.jpg";
		layout.Back = "texture/skybox/back.jpg";
		layout.Front = "texture/skybox/front.jpg";

		texture = Texture2D(DeviceInfo, "texture/container2.png");
		texture2 = Texture2D(DeviceInfo, "texture/container2_specular.png");
		cubeMapTexture = CubeMapTexture(DeviceInfo, layout);

		//skyBoxShader = SkyBoxShader(DeviceInfo, swapChainExtent, renderPass, cubeMapTexture);

		std::vector<Texture2D> TextureList;
		TextureList.emplace_back(texture);
		TextureList.emplace_back(texture2);

		MeshList = Model(mainPipeline, DeviceInfo, TextureList, vertices2, indices);
		Nanosuit = Model("Models/Nanosuit.obj");
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));
		//MeshList.emplace_back(Model(mainPipeline, DeviceInfo, swapChainExtent, renderPass, TextureList, vertices2, indices));

		//Skybox = SkyBox(DeviceInfo);
		frameBuffer = FrameBuffer(frameBufferPipeline, DeviceInfo, swapChainExtent, renderPass, PositionAttachment, NormalAttachment, AlbedoAttachment, DepthAttachment);

		createFramebuffers();
		createCommandBuffers();
		createSyncObjects();
	}

	void mainLoop() 
	{
		while (!glfwWindowShouldClose(Window.GetWindowPtr())) 
		{
			glfwPollEvents();
			drawFrame();
			UpdateMouse();
			UpdateKeyboard();
		}

		vkDeviceWaitIdle(DeviceInfo.Device);
	}

	void cleanupSwapChain() 
	{
		vkDestroyImageView(DeviceInfo.Device, PositionAttachment.AttachmentImageView, nullptr);
		vkDestroyImage(DeviceInfo.Device, PositionAttachment.AttachmentImage, nullptr);
		vkFreeMemory(DeviceInfo.Device, PositionAttachment.AttachmentImageMemory, nullptr);

		vkDestroyImageView(DeviceInfo.Device, NormalAttachment.AttachmentImageView, nullptr);
		vkDestroyImage(DeviceInfo.Device, NormalAttachment.AttachmentImage, nullptr);
		vkFreeMemory(DeviceInfo.Device, NormalAttachment.AttachmentImageMemory, nullptr);

		vkDestroyImageView(DeviceInfo.Device, AlbedoAttachment.AttachmentImageView, nullptr);
		vkDestroyImage(DeviceInfo.Device, AlbedoAttachment.AttachmentImage, nullptr);
		vkFreeMemory(DeviceInfo.Device, AlbedoAttachment.AttachmentImageMemory, nullptr);

		vkDestroyImageView(DeviceInfo.Device, DepthAttachment.AttachmentImageView, nullptr);
		vkDestroyImage(DeviceInfo.Device, DepthAttachment.AttachmentImage, nullptr);
		vkFreeMemory(DeviceInfo.Device, DepthAttachment.AttachmentImageMemory, nullptr);

		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(DeviceInfo.Device, framebuffer, nullptr);
		}

		vkFreeCommandBuffers(DeviceInfo.Device, DeviceInfo.CommandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		vkDestroyPipeline(DeviceInfo.Device, mainPipeline.ShaderPipeline, nullptr);
		vkDestroyPipelineLayout(DeviceInfo.Device, mainPipeline.ShaderPipelineLayout, nullptr);

		vkDestroyPipeline(DeviceInfo.Device, frameBufferPipeline.ShaderPipeline, nullptr);
		vkDestroyPipelineLayout(DeviceInfo.Device, frameBufferPipeline.ShaderPipelineLayout, nullptr);
		
		vkDestroyRenderPass(DeviceInfo.Device, renderPass, nullptr);

		for (auto imageView : swapChainImageViews) {
			vkDestroyImageView(DeviceInfo.Device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(DeviceInfo.Device, swapChain, nullptr);

		//for (auto mesh : MeshList)
		//{
			for (size_t i = 0; i < swapChainImages.size(); i++) {
				vkDestroyBuffer(DeviceInfo.Device, MeshList.uniformBuffers[i], nullptr);
				vkFreeMemory(DeviceInfo.Device, MeshList.uniformBuffersMemory[i], nullptr);
			}
		//}
		for (size_t i = 0; i < swapChainImages.size(); i++) {
			vkDestroyBuffer(DeviceInfo.Device, frameBuffer.LightFragmentUniformBuffers[i], nullptr);
			vkFreeMemory(DeviceInfo.Device, frameBuffer.LightFragmentUniformBuffersMemory[i], nullptr);

			vkDestroyBuffer(DeviceInfo.Device, frameBuffer.DebugUniformBuffers[i], nullptr);
			vkFreeMemory(DeviceInfo.Device, frameBuffer.DebugBuffersMemory[i], nullptr);
		}
		//for (auto mesh : MeshList)
		//{
			vkDestroyDescriptorPool(DeviceInfo.Device, MeshList.descriptorPool, nullptr);
		//}
		vkDestroyDescriptorPool(DeviceInfo.Device, frameBuffer.descriptorPool, nullptr);
	}

	void cleanup() {
		cleanupSwapChain();

		texture.Destroy();
		texture2.Destroy();
		cubeMapTexture.Destroy();

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(DeviceInfo.Device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(DeviceInfo.Device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(DeviceInfo.Device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(DeviceInfo.Device, DeviceInfo.CommandPool, nullptr);

		vkDestroyDevice(DeviceInfo.Device, nullptr);
		VulkanDebug.CleanUp(instance);
		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		Window.CleanUp();
	}

	void recreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(DeviceInfo.Device);

		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createRenderPass();

		mainPipeline.RecreatePipeline(swapChainExtent, renderPass);
		frameBufferPipeline.RecreatePipeline(swapChainExtent, renderPass);

		PositionAttachment.CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		PositionAttachment.CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);

		NormalAttachment.CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		NormalAttachment.CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);

		AlbedoAttachment.CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		AlbedoAttachment.CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);

		DepthAttachment.CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		DepthAttachment.CreateAttachmentView(VK_IMAGE_ASPECT_DEPTH_BIT);

		createFramebuffers();

		std::vector<Texture2D> TextureList;
		TextureList.emplace_back(texture);
		TextureList.emplace_back(texture2);

	/*	for (auto mesh : MeshList)
		{*/
		MeshList.CreateUniformBuffers();
		MeshList.CreateDescriptorPool();
		MeshList.CreateDescriptorSets(mainPipeline, TextureList);
	/*	}*/

		frameBuffer.CreateUniformBuffers();
		frameBuffer.CreateDescriptorPool();
		frameBuffer.CreateDescriptorSets(frameBufferPipeline, PositionAttachment.AttachmentImageView, NormalAttachment.AttachmentImageView, AlbedoAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView);

		createCommandBuffers();
	}

	void createInstance() 
	{
		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Graphics";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Vulkan Graphics";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (enableValidationLayers)
		{
			VkDebugUtilsMessengerCreateInfoEXT DebugInfo;
			VulkanDebug.CreateDebugMessengerInfo(DebugInfo);

			createInfo.enabledLayerCount = static_cast<unsigned int>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&DebugInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VkResult Result = vkCreateInstance(&createInfo, nullptr, &instance);
		if (Result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan instance. Error:" + Result);
		}
	}

	void SetUpDebugger()
	{
		if (!enableValidationLayers)
		{
			VulkanDebug.SetUpDebugger(instance);
		}

		VulkanDebug.SetUpDebugger(instance);
	}

	void createSurface() {
		if (glfwCreateWindowSurface(instance, Window.GetWindowPtr(), nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				DeviceInfo.PhysicalDevice = device;
				break;
			}
		}

		if (DeviceInfo.PhysicalDevice == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	void createLogicalDevice() {
		QueueFamilyIndices indices = findQueueFamilies(DeviceInfo.PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};
		deviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.pEnabledFeatures = &deviceFeatures;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(DeviceInfo.PhysicalDevice, &createInfo, nullptr, &DeviceInfo.Device) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(DeviceInfo.Device, indices.graphicsFamily.value(), 0, &DeviceInfo.GraphicsQueue);
		vkGetDeviceQueue(DeviceInfo.Device, indices.presentFamily.value(), 0, &presentQueue);
	}

	void createSwapChain() {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(DeviceInfo.PhysicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(DeviceInfo.PhysicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(DeviceInfo.Device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(DeviceInfo.Device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		DeviceInfo.SwapChainSize = swapChainImages.size();
		vkGetSwapchainImagesKHR(DeviceInfo.Device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
	}

	void createImageViews() {
		swapChainImageViews.resize(swapChainImages.size());

		for (uint32_t i = 0; i < swapChainImages.size(); i++) {
			swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
		}
	}

	void createRenderPass() {
		std::array<VkAttachmentDescription, 5> attachments{};

		attachments[0].format = swapChainImageFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		attachments[1].format = VK_FORMAT_R16G16B16A16_SFLOAT;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachments[2].format = VK_FORMAT_R16G16B16A16_SFLOAT;
		attachments[2].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[2].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[2].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[2].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[2].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[2].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[2].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachments[3].format = VK_FORMAT_R8G8B8A8_UNORM;
		attachments[3].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[3].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[3].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[3].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[3].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[3].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[3].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachments[4].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
		attachments[4].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[4].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[4].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[4].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[4].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[4].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[4].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		std::array<VkSubpassDescription, 2> subpassDescriptions{};

		VkAttachmentReference colorReferences[4];
		colorReferences[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[1] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[2] = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[3] = { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		VkAttachmentReference depthReference = { 4, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

		subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescriptions[0].colorAttachmentCount = 4;
		subpassDescriptions[0].pColorAttachments = colorReferences;
		subpassDescriptions[0].pDepthStencilAttachment = &depthReference;


		VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkAttachmentReference inputReferences[3];
		inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		inputReferences[2] = { 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

		uint32_t preserveAttachmentIndex = 1;

		subpassDescriptions[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescriptions[1].colorAttachmentCount = 1;
		subpassDescriptions[1].pColorAttachments = &colorReference;
		subpassDescriptions[1].pDepthStencilAttachment = &depthReference;
		subpassDescriptions[1].inputAttachmentCount = 3;
		subpassDescriptions[1].pInputAttachments = inputReferences;


		std::array<VkSubpassDependency, 3> dependencies;

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = 1;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[2].srcSubpass = 0;
		dependencies[2].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[2].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[2].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		if (vkCreateRenderPass(DeviceInfo.Device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void createFramebuffers() {
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			std::array<VkImageView, 5> attachments = {
				swapChainImageViews[i],
				PositionAttachment.AttachmentImageView,
				NormalAttachment.AttachmentImageView,
				AlbedoAttachment.AttachmentImageView,
				DepthAttachment.AttachmentImageView
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(DeviceInfo.Device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(DeviceInfo.PhysicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(DeviceInfo.Device, &poolInfo, nullptr, &DeviceInfo.CommandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics command pool!");
		}
	}

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(DeviceInfo.PhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}

	bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) {
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(DeviceInfo.Device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	void createCommandBuffers() {
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = DeviceInfo.CommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(DeviceInfo.Device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkClearValue clearValues[5];
			clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			clearValues[1].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			clearValues[2].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			clearValues[3].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
			clearValues[4].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset.x = 0;
			renderPassInfo.renderArea.offset.y = 0;
			renderPassInfo.renderArea.extent.width = WIDTH;
			renderPassInfo.renderArea.extent.height = HEIGHT;
			renderPassInfo.clearValueCount = 5;
			renderPassInfo.pClearValues = clearValues;

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			//Skybox.Draw(commandBuffers[i], skyBoxShader.descriptorSets[i], skyBoxShader.ShaderPipeline, skyBoxShader.ShaderPipelineLayout);
			/*for (auto mesh : MeshList)
			{*/
				VkBuffer vertexBuffers[] = { MeshList.vertexBuffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mainPipeline.ShaderPipeline);
				vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
				vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, mainPipeline.ShaderPipelineLayout, 0, 1, &MeshList.descriptorSets[i], 0, nullptr);
				vkCmdDraw(commandBuffers[i], 36, 1, 0, 0);
			/*}*/

			vkCmdNextSubpass(commandBuffers[i], VK_SUBPASS_CONTENTS_INLINE);
			frameBuffer.Draw(frameBufferPipeline, commandBuffers[i], i);
			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	void createSyncObjects() {
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(DeviceInfo.Device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(DeviceInfo.Device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(DeviceInfo.Device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create synchronization objects for a frame!");
			}
		}
	}

	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	/*	for (int x = 0; x < 10; x++)
		{*/
			UniformBufferObject2 ubo2 = {};
			ubo2.model = glm::mat4(1.0f);
			ubo2.model = glm::translate(ubo2.model, cubePositions[0]);
			ubo2.model = glm::rotate(ubo2.model, glm::radians(0 * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			ubo2.view = camera.GetViewMatrix();
			ubo2.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
			ubo2.proj[1][1] *= -1;

			MeshList.UpdateUniformBuffer(ubo2, currentImage);
		/*}*/

		LightingStruct ubo4 = {};
		ubo4.light.Position = glm::vec3(cos(time), -2.0f, sin(time));
		ubo4.light.Color = glm::vec3(0.0f, 1.0f, 0.0f);
		ubo4.light.Linear = 0.09f;
		ubo4.light.Quadratic = 0.032f;
		ubo4.light.Radius = 5.2f;
		ubo4.shininess = 0.5f;
		ubo4.viewPos = camera.GetCameraPos();
		//ubo4.DebugLayer = DebugLayer;

		DebugStruct debug{};
		debug.DebugLayer = DebugLayer;

		frameBuffer.UpdateUniformBuffer(ubo4, debug, currentImage);

		//SkyBoxUniformBufferObject ubo = {};
		//ubo.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		//ubo.projection = glm::perspective(glm::radians(camera.GetCameraZoom()), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
		//ubo.projection[1][1] *= -1;

		//void* data3;
		//vkMapMemory(device, skyBoxShader.uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data3);
		//memcpy(data3, &ubo, sizeof(ubo));
		//vkUnmapMemory(device, skyBoxShader.uniformBuffersMemory[currentImage]);
	}

	void drawFrame() {
		vkWaitForFences(DeviceInfo.Device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(DeviceInfo.Device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		updateUniformBuffer(imageIndex);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(DeviceInfo.Device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(DeviceInfo.Device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(DeviceInfo.GraphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	bool isDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	std::vector<const char*> getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void UpdateMouse()
	{
		if (glfwGetMouseButton(Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			glfwGetCursorPos(Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
			if (firstMouse)
			{
				lastX = MouseXPos;
				lastY = MouseYPos;
				firstMouse = false;
			}

			float xoffset = MouseXPos - lastX;
			float yoffset = lastY - MouseYPos;

			lastX = MouseXPos;
			lastY = MouseYPos;

			camera.UpdateMouse(xoffset, yoffset);
		}
		else
		{
			firstMouse = true;
		}
	}

	void UpdateKeyboard()
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
			camera.UpdateKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
			camera.UpdateKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
			camera.UpdateKeyboard(LEFT, deltaTime);
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
			camera.UpdateKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_0) == GLFW_PRESS)
			DebugLayer = 0;
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_1) == GLFW_PRESS)
			DebugLayer = 1;
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_2) == GLFW_PRESS)
			DebugLayer = 2;
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_3) == GLFW_PRESS)
			DebugLayer = 3;
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

