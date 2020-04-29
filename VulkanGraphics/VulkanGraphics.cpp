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
#include <iostream>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Image.h"

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = VulkanRenderer(Window.GetWindowPtr(), &MeshList);
	

	VulkanDevice DeviceInfo = {};
	DeviceInfo.Device = *GetDevice(renderer);
	DeviceInfo.PhysicalDevice = *GetPhysicalDevice(renderer);
	DeviceInfo.CommandPool = renderer.SubCommandPool;
	DeviceInfo.GraphicsQueue = *GetGraphicsQueue(renderer);
	DeviceInfo.SwapChainSize = GetSwapChainImageCount(renderer);
	DeviceInfo.descriptorSetLayout = *GetDescriptorSetLayout(renderer);

	texture = Texture2D(DeviceInfo, "texture/texture.jpg");
	std::vector<Texture2D> textureList = { texture, texture };

	MeshList.emplace_back(Mesh(DeviceInfo, vertices, indices, textureList));
	renderer.UpdateCommandBuffers();
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(*GetDevice(renderer));

	for (auto mesh : MeshList)
	{
		mesh.Destroy();
	}
	texture.Destroy();

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::Update()
{

}

void VulkanGraphics::UpdateCommandBuffers()
{
}

void VulkanGraphics::Draw()
{
	auto NextFrame = renderer.StartFrame(Window.GetWindowPtr());
	renderer.RunCommandBuffers.clear();
	renderer.RunCommandBuffers.emplace_back(renderer.SubCommandBuffers[NextFrame]);
	renderer.RunCommandBuffers.emplace_back(renderer.guiDebugger.GetCommandBuffers(NextFrame));
	renderer.EndFrame(Window.GetWindowPtr(), NextFrame);
}

void VulkanGraphics::MainLoop()
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
		UpdateCommandBuffers();
		Update();
		Draw();
	}
}
