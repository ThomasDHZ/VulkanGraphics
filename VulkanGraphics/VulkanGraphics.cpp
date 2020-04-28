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
	
	 RendererInfo = renderer.GetRendererInfo();

	VulkanDevice DeviceInfo = {};
	DeviceInfo.Device = RendererInfo->Device;
	DeviceInfo.PhysicalDevice = RendererInfo->PhysicalDevice;
	DeviceInfo.CommandPool = RendererInfo->SubCommandPool;
	DeviceInfo.GraphicsQueue = RendererInfo->GraphicsQueue;
	DeviceInfo.SwapChainSize = RendererInfo->SwapChainImageCount;
	DeviceInfo.descriptorSetLayout = RendererInfo->DescriptorSetLayout;

	texture = Texture2D(DeviceInfo, "texture/texture.jpg");
	std::vector<Texture2D> textureList = { texture, texture };

	MeshList.emplace_back(Mesh(DeviceInfo, vertices, indices, textureList));
	renderer.UpdateCommandBuffers();
}

VulkanGraphics::~VulkanGraphics()
{
  RendererInfo = renderer.GetRendererInfo();
	vkDeviceWaitIdle(RendererInfo->Device);

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
	renderer.guiDebugger.UpdateGuiDebugger();
}

void VulkanGraphics::UpdateCommandBuffers()
{
}

void VulkanGraphics::Draw()
{

	renderer.Draw(Window.GetWindowPtr());
}

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		UpdateCommandBuffers();
		Update();
		Draw();
	}
}
