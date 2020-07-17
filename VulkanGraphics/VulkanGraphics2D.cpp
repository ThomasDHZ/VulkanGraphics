//#include "VulkanGraphics2D.h"
//
//
//VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
//{
//	Window = VulkanWindow(Width, Height, AppName);
//	renderer = Renderer(Window.GetWindowPtr());
//	//renderer.Settings.TwoDMode = true;
//
//	MapTile[Level::SparkMan].DiffuseMap = "texture/SparkManDiffuse.bmp";
//	MapTile[Level::SparkMan].SpecularMap = "texture/SparkManSpecular.bmp";
//	MapTile[Level::SparkMan].NormalMap = "texture/SparkManSpecular.bmp";
//	MapTile[Level::SparkMan].DisplacementMap = "texture/SparkManSpecular.bmp";
//	MapTile[Level::SparkMan].EmissionMap = "texture/temp.bmp";
//	MapTile[Level::SparkMan].AlphaMap = "texture/SparkManSpecular.bmp";
//	MapTile[Level::Test].DiffuseMap = "texture/container2.png";
//	MapTile[Level::Test].SpecularMap = "texture/container2.png";
//
//	level = Level2D(renderer, MapTile[Level::SparkMan]);
//}
//
//VulkanGraphics2D::~VulkanGraphics2D()
//{
//	vkDeviceWaitIdle(*GetDevice(renderer));
//
//	level.Destroy(renderer);
//
//	renderer.DestoryVulkan();
//	Window.CleanUp();
//}
//
//void VulkanGraphics2D::UpdateCommandBuffers(uint32_t DrawFrame)
//{
//	if (renderer.UpdateCommandBuffers)
//	{
//		for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
//		{
//			VkCommandBufferInheritanceInfo InheritanceInfo = {};
//			InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
//			InheritanceInfo.renderPass = *GetRenderPass(renderer);
//			InheritanceInfo.framebuffer = renderer.SwapChainFramebuffers[i];
//
//			VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
//			BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//			BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
//			BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;
//
//			vkBeginCommandBuffer(renderer.SecondaryCommandBuffers[i], &BeginSecondaryCommandBuffer);
//			level.Draw(renderer, i);
//			if (vkEndCommandBuffer(renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
//				throw std::runtime_error("failed to record command buffer!");
//			}
//		}
//
//		renderer.UpdateCommandBuffers = false;
//	}
//
//	renderer.guiDebugger.UpdateCommandBuffers(DrawFrame, *GetRenderPass(renderer), renderer.SwapChainFramebuffers[DrawFrame]);
//}
//
//void VulkanGraphics2D::MainLoop()
//{
//	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
//	{
//		if (CompareVulkanSettings != renderer.Settings)
//		{
//			CompareVulkanSettings = renderer.Settings;
//			renderer.UpdateSwapChain(Window.GetWindowPtr());
//		}
//
//		Window.Update();
//		mouse.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);
//		keyboard.Update(Window.GetWindowPtr(), level.camera, renderer.Settings);
//		level.Update(renderer, Window.GetWindowPtr());
//		UpdateCommandBuffers(renderer.DrawFrame);
//
//		ImGui_ImplVulkan_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//		{
//			level.LevelDebug(renderer);
//		}
//
//		ImGui::Render();
//		//renderer.Draw(Window.GetWindowPtr());
//	}
//}