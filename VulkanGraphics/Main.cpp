

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
#include "Pixel.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Model.h"
#include <FileSystem.h>
#include "VulkanWindow.h"
#include "VulkanSwapChain.h"
#include "VulkanDebugger.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const int MAX_FRAMES_IN_FLIGHT = 2;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

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

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};


struct ThreadData
{
	VkCommandBuffer TreadCommandBuffer;
	VkCommandPool TreadCommandPool;

	void DeleteThread(VkDevice device)
	{
		vkDestroyCommandPool(device, TreadCommandPool, nullptr);
	}
};

struct VulkanFrame
{
	VkCommandBuffer MainCommandBuffer;
	std::vector<ThreadData> Thread;
	VkFence Fence;
	VkImage* swapChainImages;
	VkImageView swapChainImageViews;
	VkFramebuffer swapChainFramebuffers;
	VkDescriptorPool descriptorPool;

	VkDescriptorPool imGuiDescriptorPool;

	void DeleteFrame(VkDevice device)
	{
		vkDestroyFence(device, Fence, nullptr);


		for (auto thread : Thread)
		{
			thread.DeleteThread(device);
		}
	}
};

struct VulkanSemaphores
{
	VkSemaphore ImageAcquiredSemaphore;
	VkSemaphore RenderCompleteSemaphore;
};

class HelloTriangleApplication {
public:
	void run() 
	{
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	int counter = 0;
	std::vector<VulkanFrame> vulkanFrame;
	std::vector<VulkanSemaphores> vulkanSemaphores;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	Camera camera;
	VulkanWindow Window;
	std::vector<Pixel> PixelImage;

	VkCommandPool commandPool;

	VulkanSwapChain Swapchain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	Texture2D texture;
	ModelLoader modelLoader;
	std::vector<Mesh> meshList;
	std::vector<Model> modelList;

	bool showWindow = true;

	std::vector<VkDescriptorSet> descriptorSets;

	size_t currentFrame = 0;

	bool framebufferResized = false;

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		printf("VkResult %d\n", err);
		if (err < 0)
			abort();
	}

	void initVulkan() 
	{
		Window = VulkanWindow(WIDTH, HEIGHT, "Vulkan Graphics");

		vulkanFrame.resize(3);
		vulkanSemaphores.resize(3);

		for (auto& frame : vulkanFrame)
		{
			frame.Thread.emplace_back(ThreadData());
		}

		createSwapChain();

		createRenderPass();
		createDepthResources();
		createFramebuffers();

	

		createDescriptorSetLayout();
		createGraphicsPipeline();
		createCommandPool();



		VulkanDevice vulkanDevice = {};
		vulkanDevice.Device = Swapchain.Device;
		vulkanDevice.PhysicalDevice = Swapchain.PhysicalDevice;
		vulkanDevice.CommandPool = commandPool;
		vulkanDevice.GraphicsQueue = Swapchain.GraphicsQueue;
		vulkanDevice.descriptorSetLayout = descriptorSetLayout;
		vulkanDevice.SwapChainSize = vulkanFrame.size();

		modelLoader = ModelLoader(vulkanDevice, FileSystem::getPath("VulkanGraphics/Models/Nanosuit/nanosuit.obj"));

		texture = Texture2D(vulkanDevice, "texture/texture.jpg");
		std::vector<Texture2D> textures = { texture, texture };
		meshList.emplace_back(Mesh(vulkanDevice, vertices, indices, textures));
		modelList.emplace_back(Model(vulkanDevice, modelLoader.GetModelMeshs()));
		modelList[0].ModelName = "Nanosuit";
		meshList[0].MeshName = "Mesh 1";



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


		for (int x = 0; x < meshList.size(); x++)
		{
			meshList[x].MeshPosition = cubePositions[x];
		}
		for (int x = 0; x < modelList.size(); x++)
		{
			modelList[x].ModelPosition = cubePositions[x];
		}

		createDescriptorPool();
		createDescriptorSets();
		createCommandBuffers();
		createSyncObjects();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForVulkan(Window.GetWindowPtr(), true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = Swapchain.Instance;
		init_info.PhysicalDevice = Swapchain.PhysicalDevice;
		init_info.Device = Swapchain.Device;
		init_info.QueueFamily = 0;
		init_info.Queue = Swapchain.GraphicsQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = vulkanFrame[0].imGuiDescriptorPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, renderPass);

		VkCommandBuffer command_buffer = beginSingleTimeCommands(currentFrame);
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		endSingleTimeCommands(currentFrame, command_buffer);
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(Window.GetWindowPtr()))
		{

			Window.Update();
			UpdateMouse();
			UpdateKeyboard();
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				ImGui::Begin("Hello, world!");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

				if (ImGui::TreeNode("Tree"))
				{
					ImGui::Columns(1, "tree", true);
					for (int x = 0; x < meshList.size(); x++)
					{
						float translationvec4f[4] = { meshList[x].MeshPosition.x, meshList[x].MeshPosition.y, meshList[x].MeshPosition.z, 0.0f };
						float rotatevec4f[4] = { meshList[x].MeshRotate.x, meshList[x].MeshRotate.y, meshList[x].MeshRotate.z, 0.0f };
						float scalevec4f[4] = { meshList[x].MeshScale.x, meshList[x].MeshScale.y, meshList[x].MeshScale.z, 0.0f };

						bool open1 = ImGui::TreeNode((void*)(intptr_t)x, meshList[x].MeshName.c_str(), x);
						if (open1)
						{
							ImGui::SliderFloat3((meshList[x].MeshName + " Translate").c_str(), translationvec4f, -100.0f, 100.0f);
							ImGui::SliderFloat3((meshList[x].MeshName + " Rotate").c_str(), rotatevec4f, 0.0f, 1.0f);
							ImGui::SliderFloat3((meshList[x].MeshName + " Scale").c_str(), scalevec4f, 0.0f, 1.0f);
							meshList[x].MeshPosition.x = translationvec4f[0];
							meshList[x].MeshPosition.y = translationvec4f[1];
							meshList[x].MeshPosition.z = translationvec4f[2];
							meshList[x].MeshRotate.x = rotatevec4f[0];
							meshList[x].MeshRotate.y = rotatevec4f[1];
							meshList[x].MeshRotate.z = rotatevec4f[2];
							meshList[x].MeshScale.x = scalevec4f[0];
							meshList[x].MeshScale.y = scalevec4f[1];
							meshList[x].MeshScale.z = scalevec4f[2];
							ImGui::TreePop();
						}
					}
					ImGui::Columns(1);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Models"))
				{
					ImGui::Columns(1, "Models", true);
					for (int x = 0; x < modelList.size(); x++)
					{
						float translationvec4f[4] = { modelList[x].ModelPosition.x, modelList[x].ModelPosition.y, modelList[x].ModelPosition.z, 0.0f };
						float rotatevec4f[4] = { modelList[x].ModelRotate.x, modelList[x].ModelRotate.y, modelList[x].ModelRotate.z, 0.0f };
						float scalevec4f[4] = { modelList[x].ModelScale.x, modelList[x].ModelScale.y, modelList[x].ModelScale.z, 0.0f };

						bool open1 = ImGui::TreeNode((void*)(intptr_t)x, modelList[x].ModelName.c_str(), x);
						if (open1)
						{
							ImGui::SliderFloat3((modelList[x].ModelName + " Translate").c_str(), translationvec4f, -100.0f, 100.0f);
							ImGui::SliderFloat3((modelList[x].ModelName + " Rotate").c_str(), rotatevec4f, 0.0f, 1.0f);
							ImGui::SliderFloat3((modelList[x].ModelName + " Scale").c_str(), scalevec4f, 0.0f, 1.0f);
							modelList[x].ModelPosition.x = translationvec4f[0];
							modelList[x].ModelPosition.y = translationvec4f[1];
							modelList[x].ModelPosition.z = translationvec4f[2];
							modelList[x].ModelRotate.x = rotatevec4f[0];
							modelList[x].ModelRotate.y = rotatevec4f[1];
							modelList[x].ModelRotate.z = rotatevec4f[2];
							modelList[x].ModelScale.x = scalevec4f[0];
							modelList[x].ModelScale.y = scalevec4f[1];
							modelList[x].ModelScale.z = scalevec4f[2];
							//if (ImGui::TreeNode("MeshList"))
							//{
							//	ImGui::Columns(1, "MeshList", true);

							//	for (int y = 0; y < modelList[x].GetModelMeshList().size() - 1; y++)
							//	{
							//		float translationvec4f[4] = { modelList[x].GetModelMeshList()[y].MeshPosition.y, modelList[x].GetModelMeshList()[y].MeshPosition.y, modelList[x].GetModelMeshList()[y].MeshPosition.z, 0.0f };
							//		float rotatevec4f[4] = { modelList[x].GetModelMeshList()[y].MeshRotate.y, modelList[x].GetModelMeshList()[y].MeshRotate.y, modelList[x].GetModelMeshList()[y].MeshRotate.z, 0.0f };
							//		float scalevec4f[4] = { modelList[x].GetModelMeshList()[y].MeshScale.y, modelList[x].GetModelMeshList()[y].MeshScale.y, modelList[x].GetModelMeshList()[y].MeshScale.z, 0.0f };

							//		std::string a = std::to_string(y);
							//		bool open2 = ImGui::TreeNode((void*)(intptr_t)y, a.c_str(), y);
							//		if (open2)
							//		{
							//			ImGui::SliderFloat3((modelList[x].GetModelMeshList()[y].MeshName + " Translate").c_str(), translationvec4f, -100.0f, 100.0f);
							//			ImGui::SliderFloat3((modelList[x].GetModelMeshList()[y].MeshName + " Rotate").c_str(), rotatevec4f, 0.0f, 1.0f);
							//			ImGui::SliderFloat3((modelList[x].GetModelMeshList()[y].MeshName + " Scale").c_str(), scalevec4f, 0.0f, 1.0f);
							//			modelList[x].GetModelMeshList()[y].MeshPosition.x = translationvec4f[0];
							//			modelList[x].GetModelMeshList()[y].MeshPosition.y = translationvec4f[1];
							//			modelList[x].GetModelMeshList()[y].MeshPosition.z = translationvec4f[2];
							//			modelList[x].GetModelMeshList()[y].MeshRotate.x = rotatevec4f[0];
							//			modelList[x].GetModelMeshList()[y].MeshRotate.y = rotatevec4f[1];
							//			modelList[x].GetModelMeshList()[y].MeshRotate.z = rotatevec4f[2];
							//			modelList[x].GetModelMeshList()[y].MeshScale.x = scalevec4f[0];
							//			modelList[x].GetModelMeshList()[y].MeshScale.y = scalevec4f[1];
							//			modelList[x].GetModelMeshList()[y].MeshScale.z = scalevec4f[2];
							//			ImGui::TreePop();
							//		}
							//	}

							//	ImGui::Columns(1);
							//	ImGui::TreePop();
							//}
							ImGui::TreePop();
						}
					}
					ImGui::Columns(1);
					ImGui::TreePop();
				}
				ImGui::End();
			}
			ImGui::Render();
			drawFrame();
		}

		vkDeviceWaitIdle(Swapchain.Device);
	}

	void cleanupSwapChain() {
		vkDestroyImageView(Swapchain.Device, depthImageView, nullptr);
		vkDestroyImage(Swapchain.Device, depthImage, nullptr);
		vkFreeMemory(Swapchain.Device, depthImageMemory, nullptr);

		for (auto& frame : vulkanFrame) {
			vkDestroyFramebuffer(Swapchain.Device, frame.swapChainFramebuffers, nullptr);
			vkDestroyImageView(Swapchain.Device, frame.swapChainImageViews, nullptr);
			vkDestroyDescriptorPool(Swapchain.Device, frame.descriptorPool, nullptr);
			//vkDestroyDescriptorPool(device, frame.imGuiDescriptorPool, nullptr);
		}


		vkDestroyPipeline(Swapchain.Device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(Swapchain.Device, pipelineLayout, nullptr);
		vkDestroyRenderPass(Swapchain.Device, renderPass, nullptr);

		vkDestroySwapchainKHR(Swapchain.Device, Swapchain.Swapchain, nullptr);
	}

	void cleanup() {
		cleanupSwapChain();

		texture.Destroy();
		modelLoader.CleanTextureMemory();

		vkDestroyDescriptorSetLayout(Swapchain.Device, descriptorSetLayout, nullptr);
		for (auto& mesh : meshList)
		{
			mesh.ClearSwapChain();
			mesh.Destory();
		}
		for (auto& model : modelList)
		{
			model.ClearSwapChain();
			model.Destory();
		}
	
		for (auto& Semaphore : vulkanSemaphores)
		{
			vkDestroySemaphore(Swapchain.Device, Semaphore.RenderCompleteSemaphore, nullptr);
			vkDestroySemaphore(Swapchain.Device, Semaphore.ImageAcquiredSemaphore, nullptr);
		}
		for (auto& frame : vulkanFrame)
		{
			frame.DeleteFrame(Swapchain.Device);
		}
		vkDestroyCommandPool(Swapchain.Device, commandPool, nullptr);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		Swapchain.DestroyVulkan();
		Window.CleanUp();
	}

	void recreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(Swapchain.Device);

		cleanupSwapChain();

		createSwapChain();
		createRenderPass();
		createGraphicsPipeline();
		createDepthResources();
		createFramebuffers();
		createDescriptorPool();
		createDescriptorSets();
		createCommandBuffers();
	}

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	void setupDebugMessenger() {
		//if (!enableValidationLayers) return;

		//VkDebugUtilsMessengerCreateInfoEXT createInfo;
		//populateDebugMessengerCreateInfo(createInfo);

		//if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to set up debug messenger!");
		//}
	}

	void createSwapChain() {

		Swapchain = VulkanSwapChain(Window.GetWindowPtr());

		std::vector<VkImage> guiSwapChainImages;
		guiSwapChainImages.resize(Swapchain.SwapChainImageCount);
		vkGetSwapchainImagesKHR(Swapchain.Device, Swapchain.Swapchain, &Swapchain.SwapChainImageCount, guiSwapChainImages.data());

		vulkanFrame[0].swapChainImages = &Swapchain.SwapChainImages[0];
		vulkanFrame[1].swapChainImages = &Swapchain.SwapChainImages[1];
		vulkanFrame[2].swapChainImages = &Swapchain.SwapChainImages[2];

		swapChainImageFormat = Swapchain.SwapChainFormat.format;
		swapChainExtent = Swapchain.SwapChainSize;


		vulkanFrame[0].swapChainImageViews = Swapchain.SwapChainImageViews[0];
		vulkanFrame[1].swapChainImageViews = Swapchain.SwapChainImageViews[1];
		vulkanFrame[2].swapChainImageViews = Swapchain.SwapChainImageViews[2];
	}


	void createRenderPass() {
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentDescription depthAttachment = {};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef = {};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(Swapchain.Device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void createDescriptorSetLayout() {
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.pImmutableSamplers = nullptr;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayoutBinding samplerLayoutBinding2 = {};
		samplerLayoutBinding2.binding = 2;
		samplerLayoutBinding2.descriptorCount = 1;
		samplerLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding2.pImmutableSamplers = nullptr;
		samplerLayoutBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 3> bindings = { uboLayoutBinding, samplerLayoutBinding, samplerLayoutBinding2 };
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(Swapchain.Device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void createGraphicsPipeline() {
		auto vertShaderCode = readFile("shaders/vert.spv");
		auto fragShaderCode = readFile("shaders/frag.spv");

		VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		auto bindingDescription = Vertex::GetBindingDescription();
		auto attributeDescriptions = Vertex::GetAttributeDescriptions();

		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapChainExtent.width;
		viewport.height = (float)swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

		if (vkCreatePipelineLayout(Swapchain.Device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(Swapchain.Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(Swapchain.Device, fragShaderModule, nullptr);
		vkDestroyShaderModule(Swapchain.Device, vertShaderModule, nullptr);
	}

	void createFramebuffers()
	{
		for (size_t i = 0; i < vulkanFrame.size(); i++) {
			std::array<VkImageView, 2> attachments = {
				vulkanFrame[i].swapChainImageViews,
				depthImageView
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(Swapchain.Device, &framebufferInfo, nullptr, &vulkanFrame[i].swapChainFramebuffers) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void createCommandPool() {
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(Swapchain.PhysicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(Swapchain.Device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics command pool!");
		}

		for (int x = 0; x < vulkanFrame.size(); x++)
		{
			if (vkCreateCommandPool(Swapchain.Device, &poolInfo, nullptr, &vulkanFrame[x].Thread[0].TreadCommandPool) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create graphics command pool!");
			}
		}
	}

	void createDepthResources() {
		VkFormat depthFormat = findDepthFormat();

		createImage(swapChainExtent.width, swapChainExtent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
		depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(Swapchain.PhysicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}

	VkFormat findDepthFormat() {
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	bool hasStencilComponent(VkFormat format) {
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	}

	void UpdateTexture()
	{
		//VkDeviceSize imageSize = 1024 * 1024 * sizeof(Pixel);
		//memset(&PixelImage[0], 0xFF, imageSize);

		//VkBuffer stagingBuffer;
		//VkDeviceMemory stagingBufferMemory;
		//createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		//void* data;
		//vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
		//memcpy(data, PixelImage.data(), static_cast<size_t>(imageSize));
		//vkUnmapMemory(device, stagingBufferMemory);

		//transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		//copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(1024), static_cast<uint32_t>(1024));
		//transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		//vkDestroyBuffer(device, stagingBuffer, nullptr);
		//vkFreeMemory(device, stagingBufferMemory, nullptr);

		//vkDestroyImageView(device, textureImageView, nullptr);
		//CreateImageView();
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
		if (vkCreateImageView(Swapchain.Device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}

		return imageView;
	}

	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(Swapchain.Device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(Swapchain.Device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Swapchain.Device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(Swapchain.Device, image, imageMemory, 0);
	}

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(currentFrame);

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage, destinationStage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		endSingleTimeCommands(currentFrame, commandBuffer);
	}

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(currentFrame);

		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width,
			height,
			1
		};

		vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		endSingleTimeCommands(currentFrame, commandBuffer);
	}

	void createDescriptorPool() {
		std::array<VkDescriptorPoolSize, 2> poolSizes = {};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(vulkanFrame.size());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(vulkanFrame.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(vulkanFrame.size());

		for (size_t i = 0; i < vulkanFrame.size(); i++) {
			if (vkCreateDescriptorPool(Swapchain.Device, &poolInfo, nullptr, &vulkanFrame[i].descriptorPool) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor pool!");
			}
		}

		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};
		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		for (size_t i = 0; i < vulkanFrame.size(); i++)
		{
			if (vkCreateDescriptorPool(Swapchain.Device, &pool_info, nullptr, &vulkanFrame[i].imGuiDescriptorPool) != VK_SUCCESS) {
				throw std::runtime_error("failed to create descriptor pool!");
			}
		}
	}

	void createDescriptorSets() {
		for (size_t i = 0; i < vulkanFrame.size(); i++)
		{
			std::vector<VkDescriptorSetLayout> layouts(vulkanFrame.size(), descriptorSetLayout);
			VkDescriptorSetAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = vulkanFrame[i].descriptorPool;
			allocInfo.descriptorSetCount = static_cast<uint32_t>(vulkanFrame.size());
			allocInfo.pSetLayouts = layouts.data();

			descriptorSets.resize(vulkanFrame.size());
			if (vkAllocateDescriptorSets(Swapchain.Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate descriptor sets!");
			}
		}
		UpdateDescriptorSets();
	}

	void UpdateDescriptorSets()
	{
		//for (size_t i = 0; i < vulkanFrame.size(); i++)
		//{
		//	VkDescriptorBufferInfo bufferInfo = {};
		//	bufferInfo.buffer = uniformBuffers[i];
		//	bufferInfo.offset = 0;
		//	bufferInfo.range = sizeof(UniformBufferObject);

		//	VkDescriptorImageInfo imageInfo = {};
		//	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//	imageInfo.imageView = texture.textureImageView;
		//	imageInfo.sampler = texture.textureSampler;

		//	std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

		//	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[0].dstSet = descriptorSets[i];
		//	descriptorWrites[0].dstBinding = 0;
		//	descriptorWrites[0].dstArrayElement = 0;
		//	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[0].descriptorCount = 1;
		//	descriptorWrites[0].pBufferInfo = &bufferInfo;

		//	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[1].dstSet = descriptorSets[i];
		//	descriptorWrites[1].dstBinding = 1;
		//	descriptorWrites[1].dstArrayElement = 0;
		//	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//	descriptorWrites[1].descriptorCount = 1;
		//	descriptorWrites[1].pImageInfo = &imageInfo;

		//	vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		//}
	}

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(Swapchain.Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Swapchain.Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Swapchain.Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(Swapchain.Device, buffer, bufferMemory, 0);
	}

	VkCommandBuffer beginSingleTimeCommands(int frame) {
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(Swapchain.Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void endSingleTimeCommands(int frame, VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(Swapchain.GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Swapchain.GraphicsQueue);

		vkFreeCommandBuffers(Swapchain.Device, commandPool, 1, &commandBuffer);
	}

	void copyBuffer(int frame, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands(frame);

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(frame, commandBuffer);
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(Swapchain.PhysicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void createCommandBuffers() {
		for (size_t i = 0; i < vulkanFrame.size(); i++)
		{
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = commandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(Swapchain.Device, &allocInfo, &vulkanFrame[i].MainCommandBuffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}

		for (size_t i = 0; i < vulkanFrame.size(); i++)
		{
			VkCommandBufferAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.commandPool = vulkanFrame[i].Thread[0].TreadCommandPool;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
			allocInfo.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(Swapchain.Device, &allocInfo, &vulkanFrame[i].Thread[0].TreadCommandBuffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}
	}

	std::vector<VkCommandBuffer> UpdateSecondaryCommandBuffer(VulkanFrame Frame)
	{
		std::vector<VkCommandBuffer> SecondaryCommandBuffer;

		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = renderPass;
		InheritanceInfo.framebuffer = Frame.swapChainFramebuffers;

		VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
		BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;


		vkBeginCommandBuffer(Frame.Thread[0].TreadCommandBuffer, &BeginSecondaryCommandBuffer);
		for (auto& mesh : meshList)
		{
			mesh.SecBufferDraw(Frame.Thread[0].TreadCommandBuffer, BeginSecondaryCommandBuffer, graphicsPipeline, pipelineLayout, currentFrame);
		}
		for (auto& model : modelList)
		{
			model.SecBufferDraw(Frame.Thread[0].TreadCommandBuffer, BeginSecondaryCommandBuffer, graphicsPipeline, pipelineLayout, currentFrame);
		}
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Frame.Thread[0].TreadCommandBuffer);
		vkEndCommandBuffer(Frame.Thread[0].TreadCommandBuffer);

		SecondaryCommandBuffer.emplace_back(Frame.Thread[0].TreadCommandBuffer);
		return SecondaryCommandBuffer;
	}

	void createSyncObjects()
	{
		for (int x = 0; x < vulkanFrame.size(); x++)
		{
			VkFenceCreateInfo FenceInfo = {};
			FenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			FenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
			if (vkCreateFence(Swapchain.Device, &FenceInfo, nullptr, &vulkanFrame[x].Fence) != VK_SUCCESS) {
				throw std::runtime_error("failed to create fence.");
			}

			VkSemaphoreCreateInfo SemaphoreInfo = {};
			SemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			if (vkCreateSemaphore(Swapchain.Device, &SemaphoreInfo, nullptr, &vulkanSemaphores[x].ImageAcquiredSemaphore) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create ImageAcquiredSemaphore.");
			}
			if (vkCreateSemaphore(Swapchain.Device, &SemaphoreInfo, nullptr, &vulkanSemaphores[x].RenderCompleteSemaphore) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create RenderCompleteSemaphore.");
			}
		}
	}

	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		for (int x = 0; x < meshList.size(); x++)
		{
			UniformBufferObject ubo = {};
			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, meshList[x].MeshPosition);
			ubo.model = glm::rotate(ubo.model, glm::radians(time * 20.0f), meshList[x].MeshRotate);
			ubo.model = glm::scale(ubo.model, meshList[x].MeshScale);
			ubo.view = camera.GetViewMatrix();
			ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
			ubo.proj[1][1] *= -1;

			meshList[x].UpdateUniformBuffer(ubo, currentImage);
		}

		for (int x = 0; x < modelList.size(); x++)
		{
			UniformBufferObject ubo = {};
			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, modelList[x].ModelPosition);
			ubo.model = glm::rotate(ubo.model, glm::radians(time * 20.0f), modelList[x].ModelRotate);
			ubo.model = glm::scale(ubo.model, modelList[x].ModelScale);
			ubo.view = camera.GetViewMatrix();
			ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
			ubo.proj[1][1] *= -1;

			modelList[x].UpdateUniformBuffer(ubo, currentImage);

			//for (int y = 0; y < modelList[x].GetModelMeshList().size(); y++)
			//{
			//	UniformBufferObject modelUbo = {};
			//	modelUbo.model = glm::mat4(1.0f);
			//	modelUbo.model = glm::translate(modelUbo.model, modelList[x].GetModelMeshList()[y].MeshPosition);
			//	modelUbo.model = glm::rotate(modelUbo.model, glm::radians(time * 20.0f), modelList[x].GetModelMeshList()[y].MeshRotate);
			//	modelUbo.model = glm::scale(modelUbo.model, modelList[x].GetModelMeshList()[y].MeshScale);
			//	modelUbo.view = camera.GetViewMatrix();
			//	modelUbo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 100.0f);
			//	modelUbo.proj[1][1] *= -1;

			//	modelList[x].GetModelMeshList()[y].UpdateUniformBuffer(modelUbo, currentImage);
			//}
		}
	}

	void drawFrame()
	{
		std::vector<VkCommandBuffer> RunCommandBuffers = {};

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		uint32_t imageIndex;
		const VkSemaphore ImageAcquiredSemaphore = vulkanSemaphores[currentFrame].ImageAcquiredSemaphore;
		const VkSemaphore RenderCompleteSemaphore = vulkanSemaphores[currentFrame].RenderCompleteSemaphore;

		VkResult result = vkAcquireNextImageKHR(Swapchain.Device, Swapchain.Swapchain, UINT64_MAX, ImageAcquiredSemaphore, VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		currentFrame = imageIndex;
		const VulkanFrame Frame = vulkanFrame[currentFrame];

		vkWaitForFences(Swapchain.Device, 1, &Frame.Fence, VK_TRUE, UINT64_MAX);
		vkResetFences(Swapchain.Device, 1, &Frame.Fence);
	//	vkResetCommandPool(device, commandPool, 0);

		//UpdateTexture();
		//UpdateDescriptorSets();
		updateUniformBuffer(imageIndex);
		RunCommandBuffers = UpdateSecondaryCommandBuffer(Frame);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = Frame.swapChainFramebuffers;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkCommandBufferBeginInfo CommandBufferInfo = {};
		CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		vkBeginCommandBuffer(Frame.MainCommandBuffer, &CommandBufferInfo);
		vkCmdBeginRenderPass(Frame.MainCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		vkCmdExecuteCommands(Frame.MainCommandBuffer, RunCommandBuffers.size(), RunCommandBuffers.data());
		vkCmdEndRenderPass(Frame.MainCommandBuffer);
		vkEndCommandBuffer(Frame.MainCommandBuffer);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &ImageAcquiredSemaphore;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &Frame.MainCommandBuffer;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &RenderCompleteSemaphore;

		if (vkQueueSubmit(Swapchain.PresentQueue, 1, &submitInfo, Frame.Fence) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkSwapchainKHR swapChains[] = { Swapchain.Swapchain };

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &RenderCompleteSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		result = vkQueuePresentKHR(Swapchain.PresentQueue, &presentInfo);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
	}

	VkShaderModule createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Swapchain.Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
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

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, Swapchain.Surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, Swapchain.Surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, Swapchain.Surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, Swapchain.Surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, Swapchain.Surface, &presentModeCount, details.presentModes.data());
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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Swapchain.Surface, &presentSupport);

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
		{
			camera.UpdateKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.UpdateKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.UpdateKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.UpdateKeyboard(RIGHT, deltaTime);
		}
	}

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
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

