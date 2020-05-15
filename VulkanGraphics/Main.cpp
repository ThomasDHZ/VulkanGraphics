#define GLFW_INCLUDE_VULKAN
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
#include "ForwardRenderingPipeline.h"
#include "Vertex.h"
#include "VulkanSwapChain.h"
#include "InputAttachment.h"
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

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

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
	{{0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},

		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},

		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},

		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},

		{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}},

		{{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}}
};

const std::vector<uint16_t> indices = {

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
	VulkanRenderer Renderer;

	VulkanWindow Window;
	Lighter light;

	Texture2D texture;
	Mesh mesh;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;


	bool framebufferResized = false;

	void initWindow() {
		Window = VulkanWindow(WIDTH, HEIGHT, "Vulkan");
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	void initVulkan() {
		Renderer = VulkanRenderer(Window.GetWindowPtr());
		texture = Texture2D(Renderer, "texture/container2.png");
		std::vector<Texture2D> textureList = { texture, texture };
		mesh = Mesh(Renderer, vertices, indices, textureList);
		createDescriptorPool();
		createDescriptorSets();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(Window.GetWindowPtr())) {
			glfwPollEvents();
			Draw();
		}

		vkDeviceWaitIdle(Renderer.Device);
	}

	void cleanupSwapChain() {
		//vkDestroyImageView(device, depthImageView, nullptr);
		//vkDestroyImage(device, depthImage, nullptr);
		//vkFreeMemory(device, depthImageMemory, nullptr);

		//for (auto framebuffer : swapChainFramebuffers) {
		//	vkDestroyFramebuffer(device, framebuffer, nullptr);
		//}

	//	vkFreeCommandBuffers(Renderer.Device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		//vkDestroyPipeline(device, graphicsPipeline, nullptr);
		//vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	//	vkDestroyRenderPass(Renderer.Device, Renderer.RenderPass, nullptr);

		//for (auto imageView : swapChainImageViews) {
		//	vkDestroyImageView(device, imageView, nullptr);
		//}

		//vkDestroySwapchainKHR(device, swapChain, nullptr);

		//for (size_t i = 0; i < swapChainImages.size(); i++) {
		//	vkDestroyBuffer(device, uniformBuffers[i], nullptr);
		//	vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
		//}

		vkDestroyDescriptorPool(Renderer.Device, descriptorPool, nullptr);
	}

	void cleanup() {
		cleanupSwapChain();

		//vkDestroySampler(Renderer.Device, textureSampler, nullptr);
		//vkDestroyImageView(Renderer.Device, textureImageView, nullptr);

		//vkDestroyImage(Renderer.Device, textureImage, nullptr);
		//vkFreeMemory(Renderer.Device, textureImageMemory, nullptr);

		//vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

		//vkDestroyBuffer(Renderer.Device, indexBuffer, nullptr);
		//vkFreeMemory(Renderer.Device, indexBufferMemory, nullptr);

		//vkDestroyBuffer(Renderer.Device, vertexBuffer, nullptr);
		//vkFreeMemory(Renderer.Device, vertexBufferMemory, nullptr);

	/*	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(Renderer.Device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(Renderer.Device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(Renderer.Device, inFlightFences[i], nullptr);
		}*/

		//vkDestroyCommandPool(Renderer.Device, commandPool, nullptr);

		vkDestroyDevice(Renderer.Device, nullptr);

	/*	if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}*/

		vkDestroySurfaceKHR(Renderer.Instance, Renderer.Surface, nullptr);
		vkDestroyInstance(Renderer.Instance, nullptr);

		glfwDestroyWindow(Window.GetWindowPtr());

		glfwTerminate();
	}

	void recreateSwapChain() {
		int width = 0, height = 0;
		glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(Window.GetWindowPtr(), &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(Renderer.Device);

		cleanupSwapChain();

		createDescriptorPool();
		createDescriptorSets();
	}

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferImageCopy region{};
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

		endSingleTimeCommands(commandBuffer);
	}

	void createDescriptorPool() {
		std::array<VkDescriptorPoolSize, 6> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[2].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[3].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		poolSizes[4].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[4].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		poolSizes[5].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[5].descriptorCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());

		if (vkCreateDescriptorPool(Renderer.Device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	void createDescriptorSets() {
		//std::vector<VkDescriptorSetLayout> layouts(Renderer.SwapChain.GetSwapChainImageCount(), Renderer.GraphicsPipeline.ShaderPipelineDescriptorLayout);
		//VkDescriptorSetAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		//allocInfo.descriptorPool = descriptorPool;
		//allocInfo.descriptorSetCount = static_cast<uint32_t>(Renderer.SwapChain.GetSwapChainImageCount());
		//allocInfo.pSetLayouts = layouts.data();

		//descriptorSets.resize(Renderer.SwapChain.GetSwapChainImageCount());
		//if (vkAllocateDescriptorSets(Renderer.Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		//	throw std::runtime_error("failed to allocate descriptor sets!");
		//}

		//for (size_t i = 0; i < Renderer.SwapChain.GetSwapChainImageCount(); i++) {
		//	VkDescriptorBufferInfo bufferInfo{};
		//	bufferInfo.buffer = uniformBuffers[i];
		//	bufferInfo.offset = 0;
		//	bufferInfo.range = sizeof(UniformBufferObject);

		//	VkDescriptorBufferInfo MaterialbufferInfo{};
		//	MaterialbufferInfo.buffer = MaterialuniformBuffers[i];
		//	MaterialbufferInfo.offset = 0;
		//	MaterialbufferInfo.range = sizeof(Material);

		//	VkDescriptorBufferInfo AmbientLightbufferInfo{};
		//	AmbientLightbufferInfo.buffer = AmbientLightUniformBuffers[i];
		//	AmbientLightbufferInfo.offset = 0;
		//	AmbientLightbufferInfo.range = sizeof(AmbientLightUniformBuffer);

		//	VkDescriptorBufferInfo LightbufferInfo{};
		//	LightbufferInfo.buffer = LightuniformBuffers[i];
		//	LightbufferInfo.offset = 0;
		//	LightbufferInfo.range = sizeof(Lighter);

		//	std::array<VkWriteDescriptorSet, 4> descriptorWrites{};

		//	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[0].dstSet = descriptorSets[i];
		//	descriptorWrites[0].dstBinding = 0;
		//	descriptorWrites[0].dstArrayElement = 0;
		//	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[0].descriptorCount = 1;
		//	descriptorWrites[0].pBufferInfo = &bufferInfo;

		//	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[1].dstSet = descriptorSets[i];
		//	descriptorWrites[1].dstBinding = 3;
		//	descriptorWrites[1].dstArrayElement = 0;
		//	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[1].descriptorCount = 1;
		//	descriptorWrites[1].pBufferInfo = &LightbufferInfo;

		//	descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[2].dstSet = descriptorSets[i];
		//	descriptorWrites[2].dstBinding = 4;
		//	descriptorWrites[2].dstArrayElement = 0;
		//	descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[2].descriptorCount = 1;
		//	descriptorWrites[2].pBufferInfo = &LightbufferInfo;

		//	descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		//	descriptorWrites[3].dstSet = descriptorSets[i];
		//	descriptorWrites[3].dstBinding = 5;
		//	descriptorWrites[3].dstArrayElement = 0;
		//	descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		//	descriptorWrites[3].descriptorCount = 1;
		//	descriptorWrites[3].pBufferInfo = &LightbufferInfo;
		//	vkUpdateDescriptorSets(Renderer.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		//}
	}

	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(Renderer.Device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Renderer.Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(Renderer.Device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(Renderer.Device, buffer, bufferMemory, 0);
	}

	VkCommandBuffer beginSingleTimeCommands() {
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = Renderer.MainCommandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(Renderer.Device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void endSingleTimeCommands(VkCommandBuffer commandBuffer) {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(Renderer.GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(Renderer.GraphicsQueue);

		vkFreeCommandBuffers(Renderer.Device, Renderer.MainCommandPool, 1, &commandBuffer);
	}

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(Renderer.PhysicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("failed to find suitable memory type!");
	}

	void updateUniformBuffer(uint32_t currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		PositionMatrix ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), Renderer.SwapChain.GetSwapChainResolution().width / (float)Renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10.0f);
		ubo.proj[1][1] *= -1;

		AmbientLightUniformBuffer buff{};
		Lighter light{};
		light.objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		light.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		light.lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		light.viewPos = glm::vec3(0.0f, 0.0f, 3.0f);


		mesh.UpdateUniformBuffer(Renderer, ubo, buff, light, currentImage);


		//mesh.UpdateUniformBuffer
		//void* data;
		//vkMapMemory(Renderer.Device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
		//memcpy(data, &ubo, sizeof(ubo));
		//vkUnmapMemory(Renderer.Device, uniformBuffersMemory[currentImage]);

		//light.objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
		//light.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		//light.lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
		//light.viewPos = glm::vec3(0.0f, 0.0f, 3.0f);

		//void* data2;
		//vkMapMemory(Renderer.Device, LightuniformBuffersMemory[currentImage], 0, sizeof(light), 0, &data2);
		//memcpy(data2, &light, sizeof(light));
		//vkUnmapMemory(Renderer.Device, LightuniformBuffersMemory[currentImage]);
	}

	void UpdateCommandBuffers(uint32_t NextFrameIndex)
	{
		if (Renderer.UpdateCommandBuffers)
		{
			for (size_t i = 0; i < Renderer.SwapChain.GetSwapChainImageCount(); i++)
			{
				VkCommandBufferInheritanceInfo InheritanceInfo = {};
				InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
				InheritanceInfo.renderPass = Renderer.RenderPass;
				InheritanceInfo.framebuffer = Renderer.SwapChainFramebuffers[i];

				VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
				BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
				BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;


				vkBeginCommandBuffer(Renderer.SecondaryCommandBuffers[i], &BeginSecondaryCommandBuffer);
				mesh.Draw(Renderer, i);
				if (vkEndCommandBuffer(Renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
					throw std::runtime_error("failed to record command buffer!");
				}
			}

			Renderer.UpdateCommandBuffers = false;
		}

		//guiDebugger.UpdateCommandBuffers(NextFrameIndex, *GetRenderPass(renderer), renderer.SwapChainFramebuffers[NextFrameIndex]);
	}

	void Draw()
	{
		//if (CompareVulkanSettings != renderer.Settings)
		//{
		//	CompareVulkanSettings = renderer.Settings;
		//	renderer.UpdateSwapChain(Window.GetWindowPtr());
		//}

		auto NextFrameIndex = Renderer.StartFrame(Window.GetWindowPtr());
		updateUniformBuffer(NextFrameIndex);
		UpdateCommandBuffers(NextFrameIndex);
		Renderer.RunCommandBuffers.clear();
		Renderer.RunCommandBuffers.emplace_back(Renderer.SecondaryCommandBuffers[NextFrameIndex]);
		//renderer.RunCommandBuffers.emplace_back(guiDebugger.GetCommandBuffers(NextFrameIndex));
		Renderer.EndFrame(Window.GetWindowPtr(), NextFrameIndex);
	}

	VkShaderModule createShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Renderer.Device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_R8G8B8A8_UNORM && availableFormat.colorSpace == 
				VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
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
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, Renderer.Surface, &presentSupport);

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

