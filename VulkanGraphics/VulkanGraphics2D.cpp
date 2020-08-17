#include "VulkanGraphics2D.h"


VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	//renderer.Settings.TwoDMode = true;

	MapTile[Level::SparkMan].DiffuseMap = "texture/SparkManDiffuse.bmp";
	MapTile[Level::SparkMan].SpecularMap = "texture/SparkManSpecular.bmp";
	MapTile[Level::SparkMan].NormalMap = "texture/SparkManSpecular.bmp";
	MapTile[Level::SparkMan].DisplacementMap = "texture/SparkManSpecular.bmp";
	MapTile[Level::SparkMan].EmissionMap = "texture/temp.bmp";
	MapTile[Level::SparkMan].AlphaMap = "texture/SparkManSpecular.bmp";
	MapTile[Level::Test].DiffuseMap = "texture/container2.png";
	MapTile[Level::Test].SpecularMap = "texture/container2.png";

	level = Level2D(renderer, MapTile[Level::SparkMan]);
}

VulkanGraphics2D::~VulkanGraphics2D()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	level.Destroy(renderer);

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics2D::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		if (CompareVulkanSettings != renderer.Settings)
		{
			CompareVulkanSettings = renderer.Settings;
			renderer.UpdateSwapChain(Window.GetWindowPtr());
		}

		Window.Update();
		mouse.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);
		keyboard.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);
		level.Update(renderer, Window.GetWindowPtr());
		//UpdateCommandBuffers(renderer.DrawFrame);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			level.LevelDebug(renderer);
		}

		ImGui::Render();
		renderer.Draw(Window.GetWindowPtr(), MeshList, Skybox, debugLightMesh);
	}
}