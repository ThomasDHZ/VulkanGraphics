#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION

#include <GLFW/glfw3.h>

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
#include "VulkanRenderer.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"
#include "Texture2D.h"
#include "Mesh.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

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

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:

	VulkanRenderer renderer;

	GLFWwindow* window;

	Texture2D texture;
	Mesh mesh;

	size_t currentFrame = 0;

	VkDescriptorPool ImGuiDescriptorPool;
	VkCommandPool ImGuiCommandPool;
	std::vector<VkCommandBuffer> ImGuiCommandBuffers;

	bool framebufferResized = false;

	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void initVulkan() 
	{
		renderer = VulkanRenderer(window);

		VulkanDevice DeviceInfo = {};
		DeviceInfo.Device = renderer.Device;
		DeviceInfo.PhysicalDevice = renderer.PhysicalDevice;
		DeviceInfo.CommandPool = renderer.commandPool;
		DeviceInfo.GraphicsQueue = renderer.GraphicsQueue;
		DeviceInfo.SwapChainSize = renderer.GetSwapChainImageCount();
		DeviceInfo.descriptorSetLayout = renderer.GraphicsPipeline.ShaderPipelineDescriptorLayout;

		texture = Texture2D(DeviceInfo, "texture/texture.jpg");
		std::vector<Texture2D> textureList = { texture, texture };
		mesh = Mesh(DeviceInfo, vertices, indices, textureList);

		for (size_t i = 0; i < renderer.commandBuffers.size(); i++)
		{
			VkCommandBufferInheritanceInfo InheritanceInfo = {};
			InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			InheritanceInfo.renderPass = renderer.RenderPass;
			InheritanceInfo.framebuffer = renderer.swapChainFramebuffers[i];

			VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
			BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

			vkBeginCommandBuffer(renderer.commandBuffers[i], &BeginSecondaryCommandBuffer);
			mesh.SecBufferDraw(renderer.commandBuffers[i], BeginSecondaryCommandBuffer, renderer.GraphicsPipeline.ShaderPipeline, renderer.GraphicsPipeline.ShaderPipelineLayout, i);
			if (vkEndCommandBuffer(renderer.commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		ImGuiCommandBuffers.resize(renderer.GetSwapChainImageCount());

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = renderer.GraphicsFamily;

		if (vkCreateCommandPool(renderer.Device, &poolInfo, nullptr, &ImGuiCommandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphics command pool!");
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

		if (vkCreateDescriptorPool(renderer.Device, &pool_info, nullptr, &ImGuiDescriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}

		VkCommandBufferAllocateInfo allocInfo2{};
		allocInfo2.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo2.commandPool = ImGuiCommandPool;
		allocInfo2.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo2.commandBufferCount = (uint32_t)ImGuiCommandBuffers.size();

		if (vkAllocateCommandBuffers(renderer.Device, &allocInfo2, ImGuiCommandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = renderer.Instance;
		init_info.PhysicalDevice = renderer.PhysicalDevice;
		init_info.Device = renderer.Device;
		init_info.QueueFamily = renderer.GraphicsFamily;
		init_info.Queue = renderer.GraphicsQueue;
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = ImGuiDescriptorPool;
		init_info.Allocator = nullptr;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, renderer.RenderPass);

		VkCommandBuffer command_buffer = VulkanBufferManager::beginSingleTimeCommands(renderer.Device, renderer.commandPool);
		ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
		VulkanBufferManager::endSingleTimeCommands(renderer.Device, command_buffer, renderer.commandPool, renderer.GraphicsQueue);
	}

	void mainLoop() 
	{
		while (!glfwWindowShouldClose(window)) 
		{
			glfwPollEvents();
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				ImGui::Begin("Hello, world!");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			ImGui::Render();
			drawFrame();
		}
		vkDeviceWaitIdle(renderer.Device);
	}

	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		printf("VkResult %d\n", err);
		if (err < 0)
			abort();
	}


	void cleanupSwapChain() 
	{
		renderer.DepthAttachment.UpdateFrameBuffer(renderer.Device);

		for (auto framebuffer : renderer.swapChainFramebuffers) {
			vkDestroyFramebuffer(renderer.Device, framebuffer, nullptr);
		}

		vkFreeCommandBuffers(renderer.Device, renderer.commandPool, static_cast<uint32_t>(renderer.commandBuffers.size()), renderer.commandBuffers.data());
		vkDestroyPipeline(renderer.Device, renderer.GraphicsPipeline.ShaderPipeline, nullptr);
		vkDestroyPipelineLayout(renderer.Device, renderer.GraphicsPipeline.ShaderPipelineLayout, nullptr);

		for (auto imageView : renderer.GetSwapChainImageViews()) 
		{
			vkDestroyImageView(renderer.Device, imageView, nullptr);
		}

		vkDestroyCommandPool(renderer.Device, renderer.MainCommandPool, nullptr);
		vkDestroyCommandPool(renderer.Device, renderer.commandPool, nullptr);

		vkDestroySwapchainKHR(renderer.Device, renderer.GetSwapChain(), nullptr);
	}

	void cleanup() {
		cleanupSwapChain();

		vkDestroyRenderPass(renderer.Device, renderer.RenderPass, nullptr);

		for (size_t i = 0; i < renderer.GetSwapChainImageCount(); i++) 
		{
			vkDestroyBuffer(renderer.Device, mesh.uniformBuffers[i], nullptr);
			vkFreeMemory(renderer.Device, mesh.uniformBuffersMemory[i], nullptr);
		}

		mesh.Destory();

		vkDestroyDescriptorPool(renderer.Device, ImGuiDescriptorPool, nullptr);

		texture.Destroy();

		vkDestroyDescriptorSetLayout(renderer.Device, renderer.GraphicsPipeline.ShaderPipelineDescriptorLayout, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(renderer.Device, renderer.renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(renderer.Device, renderer.imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(renderer.Device, renderer.inFlightFences[i], nullptr);
		}
		
		vkDestroyCommandPool(renderer.Device, ImGuiCommandPool, nullptr);

		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		vkDestroyDevice(renderer.Device, nullptr);

		renderer.VulkanDebug.CleanUp(renderer.Instance);

		vkDestroySurfaceKHR(renderer.Instance, renderer.Surface, nullptr);
		vkDestroyInstance(renderer.Instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void recreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(window, &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(renderer.Device);

		cleanupSwapChain();

		renderer.UpdateSwapChain(window);

		for (size_t i = 0; i < renderer.commandBuffers.size(); i++)
		{
			VkCommandBufferInheritanceInfo InheritanceInfo = {};
			InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			InheritanceInfo.renderPass = renderer.RenderPass;
			InheritanceInfo.framebuffer = renderer.swapChainFramebuffers[i];

			VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
			BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

			vkBeginCommandBuffer(renderer.commandBuffers[i], &BeginSecondaryCommandBuffer);
			mesh.SecBufferDraw(renderer.commandBuffers[i], BeginSecondaryCommandBuffer, renderer.GraphicsPipeline.ShaderPipeline, renderer.GraphicsPipeline.ShaderPipelineLayout, i);
			if (vkEndCommandBuffer(renderer.commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}
	
	void UpdateImGui(uint32_t currentFrame)
	{
		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = renderer.RenderPass;
		InheritanceInfo.framebuffer = renderer.swapChainFramebuffers[currentFrame];

		VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
		BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

		vkBeginCommandBuffer(ImGuiCommandBuffers[currentFrame], &BeginSecondaryCommandBuffer);
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), ImGuiCommandBuffers[currentFrame]);
		if (vkEndCommandBuffer(ImGuiCommandBuffers[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), renderer.GetSwapChainResolution().width / (float)renderer.GetSwapChainResolution().height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		mesh.UpdateUniformBuffer(ubo, currentImage);
	}

	void drawFrame() {
		vkWaitForFences(renderer.Device, 1, &renderer.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(renderer.Device, renderer.GetSwapChain(), UINT64_MAX, renderer.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		updateUniformBuffer(imageIndex);
		UpdateImGui(imageIndex);
		if (renderer.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
			vkWaitForFences(renderer.Device, 1, &renderer.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}
		renderer.imagesInFlight[imageIndex] = renderer.inFlightFences[currentFrame];

		std::vector<VkCommandBuffer> RunCommandBuffers = {};
		RunCommandBuffers.emplace_back(renderer.commandBuffers[imageIndex]);
		RunCommandBuffers.emplace_back(ImGuiCommandBuffers[imageIndex]);

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderer.RenderPass;
		renderPassInfo.framebuffer = renderer.swapChainFramebuffers[imageIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = renderer.GetSwapChainResolution();
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		VkCommandBufferBeginInfo CommandBufferInfo = {};
		CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		vkBeginCommandBuffer(renderer.MainCommandBuffer[imageIndex], &CommandBufferInfo);
		vkCmdBeginRenderPass(renderer.MainCommandBuffer[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
		vkCmdExecuteCommands(renderer.MainCommandBuffer[imageIndex], RunCommandBuffers.size(), RunCommandBuffers.data());
		vkCmdEndRenderPass(renderer.MainCommandBuffer[imageIndex]);
		vkEndCommandBuffer(renderer.MainCommandBuffer[imageIndex]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { renderer.imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &renderer.MainCommandBuffer[imageIndex];

		VkSemaphore signalSemaphores[] = { renderer.renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(renderer.Device, 1, &renderer.inFlightFences[currentFrame]);

		if (vkQueueSubmit(renderer.GraphicsQueue, 1, &submitInfo, renderer.inFlightFences[currentFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { renderer.GetSwapChain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(renderer.PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
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

