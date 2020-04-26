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
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

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
		initVulkan();
		mainLoop();
		cleanup();
	}

private:

	VulkanWindow Window;
	VulkanRenderer renderer;

	Texture2D texture;
	Mesh mesh;


	void initVulkan() 
	{
		Window = VulkanWindow(WIDTH, HEIGHT, "Vulkan Graphics");
		renderer = VulkanRenderer(Window.GetWindowPtr());

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
	}

	void mainLoop() 
	{
		while (!glfwWindowShouldClose(Window.GetWindowPtr()))
		{
			Window.Update();
			ImGui_ImplVulkan_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			{
				ImGui::Begin("Hello, world!");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			ImGui::Render();
			renderer.Draw(Window.GetWindowPtr(), mesh);
		}
		vkDeviceWaitIdle(renderer.Device);
	}

	void cleanup() 
	{
		mesh.Destroy();
		texture.Destroy();

		renderer.DestoryVulkan();
		Window.CleanUp();
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

