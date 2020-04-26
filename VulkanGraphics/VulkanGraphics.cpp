#include "VulkanGraphics.h"

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
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Image.h"

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, "Vulkan Graphics");
	renderer = VulkanRenderer(Window.GetWindowPtr());
	const VulkanRendererInfo RendererInfo = renderer.GetRendererInfo();


	VulkanDevice DeviceInfo = {};
	DeviceInfo.Device = RendererInfo.Device;
	DeviceInfo.PhysicalDevice = RendererInfo.PhysicalDevice;
	DeviceInfo.CommandPool = renderer.SubCommandPool;
	DeviceInfo.GraphicsQueue = RendererInfo.GraphicsQueue;
	DeviceInfo.SwapChainSize = RendererInfo.SwapChainImageCount;
	DeviceInfo.descriptorSetLayout = RendererInfo.DescriptorSetLayout;

	texture = Texture2D(DeviceInfo, "texture/texture.jpg");
	std::vector<Texture2D> textureList = { texture, texture };
	mesh = Mesh(DeviceInfo, vertices, indices, textureList);

	for (size_t i = 0; i < renderer.SubCommandBuffers.size(); i++)
	{
		VkCommandBufferInheritanceInfo InheritanceInfo = {};
		InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		InheritanceInfo.renderPass = RendererInfo.RenderPass;
		InheritanceInfo.framebuffer = renderer.swapChainFramebuffers[i];

		VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
		BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

		vkBeginCommandBuffer(renderer.SubCommandBuffers[i], &BeginSecondaryCommandBuffer);
		mesh.SecBufferDraw(renderer.SubCommandBuffers[i], BeginSecondaryCommandBuffer, RendererInfo.ShaderPipeline, RendererInfo.ShaderPipelineLayout, i);
		if (vkEndCommandBuffer(renderer.SubCommandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

VulkanGraphics::~VulkanGraphics()
{
	const VulkanRendererInfo RendererInfo = renderer.GetRendererInfo();
	vkDeviceWaitIdle(RendererInfo.Device);

	mesh.Destroy();
	texture.Destroy();

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		renderer.guiDebugger.UpdateGuiDebugger();
		renderer.Draw(Window.GetWindowPtr(), mesh);
	}
}
