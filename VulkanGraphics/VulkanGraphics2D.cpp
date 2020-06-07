#include "VulkanGraphics2D.h"


VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = Renderer(Window.GetWindowPtr());
	//renderer.Settings.TwoDMode = true;

	MapTile[Level::SparkMan].DiffuseMap = "texture/SparkManDiffuse2048.bmp";
	MapTile[Level::SparkMan].SpecularMap = "texture/SparkManSpec2048.bmp";
	MapTile[Level::SparkMan].NormalMap = "texture/SparkManNormal2048.bmp";
	MapTile[Level::SparkMan].AlphaMap = "texture/SparkManAlpha2048.bmp";
	MapTile[Level::Test].DiffuseMap = "texture/container2.png";
	MapTile[Level::Test].SpecularMap = "texture/container2.png";

	level = Level2D(renderer, MapTile[Level::SparkMan]);

	InitializeGUIDebugger();
}

VulkanGraphics2D::~VulkanGraphics2D()
{
	vkDeviceWaitIdle(*GetDevice(renderer));

	level.Destroy(renderer);

	guiDebugger.ShutDown(*GetDevice(renderer));
	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics2D::InitializeGUIDebugger()
{
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = *GetInstance(renderer);
	init_info.PhysicalDevice = *GetPhysicalDevice(renderer);
	init_info.Device = *GetDevice(renderer);
	init_info.QueueFamily = 0;
	init_info.Queue = *GetGraphicsQueue(renderer);
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = GetSwapChainMinImageCount(renderer);
	init_info.ImageCount = GetSwapChainImageCount(renderer);

	guiDebugger = GUIDebugger(init_info, Window.GetWindowPtr(), *GetRenderPass(renderer));
}

void VulkanGraphics2D::Update(uint32_t DrawFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	level.Update(renderer);
}

void VulkanGraphics2D::UpdateCommandBuffers(uint32_t DrawFrame)
{
	if (renderer.UpdateCommandBuffers)
	{
		for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
		{
			VkCommandBufferInheritanceInfo InheritanceInfo = {};
			InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			InheritanceInfo.renderPass = *GetRenderPass(renderer);
			InheritanceInfo.framebuffer = renderer.SwapChainFramebuffers[i];

			VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
			BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

			vkBeginCommandBuffer(renderer.SecondaryCommandBuffers[i], &BeginSecondaryCommandBuffer);
			level.Draw(renderer, i);
			if (vkEndCommandBuffer(renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		renderer.UpdateCommandBuffers = false;
	}

	guiDebugger.UpdateCommandBuffers(DrawFrame, *GetRenderPass(renderer), renderer.SwapChainFramebuffers[DrawFrame]);
}

void VulkanGraphics2D::Draw()
{
	if (CompareVulkanSettings != renderer.Settings)
	{
		CompareVulkanSettings = renderer.Settings;
		renderer.UpdateSwapChain(Window.GetWindowPtr());
	}

	renderer.StartFrame(Window.GetWindowPtr());
	Update(renderer.DrawFrame);
	UpdateCommandBuffers(renderer.DrawFrame);
	renderer.RunCommandBuffers.clear();
	renderer.RunCommandBuffers.emplace_back(renderer.SecondaryCommandBuffers[renderer.DrawFrame]);
	renderer.RunCommandBuffers.emplace_back(guiDebugger.GetCommandBuffers(renderer.DrawFrame));
	renderer.EndFrame(Window.GetWindowPtr());
}

void VulkanGraphics2D::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		level.Update(renderer);

		mouse.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);
		keyboard.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			level.LevelDebug(renderer);
		}

		ImGui::Render();
		Draw();
	}
}