#include "VulkanGraphics2.h"

int main() {
	VulkanGraphics2 app = VulkanGraphics2(800, 600, "VulkanGraphics");

	try {
		app.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <stb_image.h>
//
//#include <iostream>
//#include <fstream>
//#include <stdexcept>
//#include <algorithm>
//#include <chrono>
//#include <vector>
//#include <cstring>
//#include <cstdlib>
//#include <cstdint>
//#include <array>
//#include <optional>
//#include <set>
//#include "ForwardRenderingPipeline.h"
//#include "Vertex.h"
//#include "VulkanSwapChain.h"
//#include "InputAttachment.h"
//#include "VulkanRenderer.h"
//#include "Texture2D.h"
//#include "Mesh.h"
//#include "VulkanWindow.h"
//#include "GUIDebugger.h"
//#include "Skybox.h"
//#include "Camera.h"
//
//const uint32_t WIDTH = 800;
//const uint32_t HEIGHT = 600;
//
//#ifdef NDEBUG
//const bool enableValidationLayers = false;
//#else
//const bool enableValidationLayers = true;
//#endif
//
//const std::vector<Vertex> vertices = {
//	{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//	{{0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//		{{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//		{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
//
//		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//		 {{0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//		{{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
//
//		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
//		{{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
//		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
//		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
//		{{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
//		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
//
//		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
//		 {{0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
//		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
//		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
//		 {{0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
//		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
//
//		{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
//		 {{0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}},
//		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
//		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
//		{{-0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}},
//		{{-0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}},
//
//		{{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
//		{{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
//		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
//		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
//		{{-0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
//		{{-0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}}
//};
//
//const std::vector<uint16_t> indices = {
//
//};
//
//class HelloTriangleApplication {
//public:
//	void run() {
//		initWindow();
//		initVulkan();
//		mainLoop();
//		cleanup();
//	}
//
//private:
//	Camera camera;
//	VulkanSettings CompareVulkanSettings;
//	VulkanRenderer Renderer;
//	GUIDebugger guiDebugger;
//
//	VulkanWindow Window;
//	Lighter light;
//
//	Texture2D texture;
//	Mesh mesh;
//
//	SkyBox Skybox;
//	CubeMapTexture SkyboxTexture;
//
//	float deltaTime = 0.0f;
//	float lastFrame = 0.0f;
//	float lastX;
//	float lastY;
//	bool firstMouse;
//	double MouseXPos;
//	double MouseYPos;
//
//	bool framebufferResized = false;
//
//	void initWindow() {
//		Window = VulkanWindow(WIDTH, HEIGHT, "Vulkan");
//	}
//
//	void initVulkan() {
//		Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
//		Renderer = VulkanRenderer(Window.GetWindowPtr());
//		texture = Texture2D(Renderer, "texture/container2.png");
//		std::vector<Texture2D> textureList = { texture, texture };
//		mesh = Mesh(Renderer, vertices, indices, textureList);
//
//		CubeMapLayout layout;
//		layout.Left = "texture/skybox/left.jpg";
//		layout.Right = "texture/skybox/right.jpg";
//		layout.Top = "texture/skybox/top.jpg";
//		layout.Bottom = "texture/skybox/bottom.jpg";
//		layout.Back = "texture/skybox/back.jpg";
//		layout.Front = "texture/skybox/front.jpg";
//		SkyboxTexture = CubeMapTexture(Renderer, layout);
//
//		Skybox = SkyBox(Renderer, SkyboxTexture);
//
//		InitializeGUIDebugger();
//	}
//
//	void InitializeGUIDebugger()
//	{
//		ImGui_ImplVulkan_InitInfo init_info = {};
//		init_info.Instance = Renderer.Instance;
//		init_info.PhysicalDevice = Renderer.PhysicalDevice;
//		init_info.Device = Renderer.Device;
//		init_info.QueueFamily = 0;
//		init_info.Queue = Renderer.GraphicsQueue;
//		init_info.PipelineCache = VK_NULL_HANDLE;
//		init_info.Allocator = nullptr;
//		init_info.MinImageCount = Renderer.SwapChain.GetSwapChainMinImageCount();
//		init_info.ImageCount = Renderer.SwapChain.GetSwapChainImageCount();
//
//		guiDebugger = GUIDebugger(init_info, Window.GetWindowPtr(), Renderer.RenderPass);
//	}
//
//	void mainLoop() {
//		while (!glfwWindowShouldClose(Window.GetWindowPtr())) {
//			Window.Update();
//			UpdateMouse();
//			UpdateKeyboard();
//			ImGui_ImplVulkan_NewFrame();
//			ImGui_ImplGlfw_NewFrame();
//			ImGui::NewFrame();
//			{
//				ImGui::Begin("Hello, world!");
//				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//				ImGui::Checkbox("MeshView", &Renderer.Settings.ShowMeshLines);
//				ImGui::Checkbox("Show SkyBox", &Renderer.Settings.ShowSkyBox);
//				//ImGui::SliderFloat("float", Ambiant.GetColorStrengthPtr(), 0.0f, 1.0f);
//				//ImGui::ColorEdit3("Ambiant color", (float*)Ambiant.GetColorPtr());
//				//ImGui::ColorEdit3("Position", (float*)&lighter.Position);
//				//ImGui::ColorEdit3("Color", (float*)&lighter.Color);
//				//ImGui::ColorEdit3("CameraPosition", (float*)&lighter.viewPos);
//				ImGui::End();
//			}
//			ImGui::Render();
//			Draw();
//		}
//
//		vkDeviceWaitIdle(Renderer.Device);
//	}
//
//	void cleanup() {
//
//		vkDeviceWaitIdle(Renderer.Device);
//
//		//modelLoader.CleanTextureMemory(renderer);
//		//for (auto mesh : MeshList)
//		//{
//		//	mesh.Destroy(renderer);
//		//}
//		//for (auto model : ModelList)
//		//{
//		//	model.Destroy(renderer);
//		//}
//		//Skybox.Destory(renderer);
//		//Ambiant.Destroy(renderer);
//		texture.Destroy(Renderer);
//		mesh.Destroy(Renderer);
//		SkyboxTexture.Destroy(Renderer);
//
//		guiDebugger.ShutDown(Renderer.Device);
//		Renderer.DestoryVulkan();
//		Window.CleanUp();
//	}
//
//	void updateUniformBuffer(uint32_t currentImage) {
//		static auto startTime = std::chrono::high_resolution_clock::now();
//
//		auto currentTime = std::chrono::high_resolution_clock::now();
//		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
//
//		PositionMatrix ubo{};
//		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//		ubo.view = camera.GetViewMatrix();
//		ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), Renderer.SwapChain.GetSwapChainResolution().width / (float)Renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
//		ubo.proj[1][1] *= -1;
//
//		AmbientLightUniformBuffer buff{};
//		Lighter light{};
//		light.objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
//		light.lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
//		light.lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
//		light.viewPos = glm::vec3(0.0f, 0.0f, 3.0f);
//
//		mesh.UpdateUniformBuffer(Renderer, ubo, buff, light, currentImage);
//
//		SkyBoxUniformBufferObject skyUbo = {};
//		skyUbo.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
//		skyUbo.projection = glm::perspective(glm::radians(camera.GetCameraZoom()), Renderer.SwapChain.GetSwapChainResolution().width / (float)Renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
//		skyUbo.projection[1][1] *= -1;
//
//		Skybox.UpdateUniformBuffer(Renderer, skyUbo, currentImage);
//	}
//
//	void UpdateCommandBuffers(uint32_t NextFrameIndex)
//	{
//		if (Renderer.UpdateCommandBuffers)
//		{
//			for (size_t i = 0; i < Renderer.SwapChain.GetSwapChainImageCount(); i++)
//			{
//				VkCommandBufferInheritanceInfo InheritanceInfo = {};
//				InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
//				InheritanceInfo.renderPass = Renderer.RenderPass;
//				InheritanceInfo.framebuffer = Renderer.SwapChainFramebuffers[i];
//
//				VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
//				BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//				BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
//				BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;
//
//
//				vkBeginCommandBuffer(Renderer.SecondaryCommandBuffers[i], &BeginSecondaryCommandBuffer);
//				mesh.Draw(Renderer, i);
//				if (Renderer.Settings.ShowSkyBox)
//				{
//					Skybox.Draw(Renderer, i);
//				}
//				if (vkEndCommandBuffer(Renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
//					throw std::runtime_error("failed to record command buffer!");
//				}
//			}
//
//			Renderer.UpdateCommandBuffers = false;
//		}
//
//		guiDebugger.UpdateCommandBuffers(NextFrameIndex, Renderer.RenderPass, Renderer.SwapChainFramebuffers[NextFrameIndex]);
//	}
//
//	void Draw()
//	{
//		if (CompareVulkanSettings != Renderer.Settings)
//		{
//			CompareVulkanSettings = Renderer.Settings;
//			Renderer.UpdateSwapChain(Window.GetWindowPtr());
//		}
//
//		auto NextFrameIndex = Renderer.StartFrame(Window.GetWindowPtr());
//		updateUniformBuffer(NextFrameIndex);
//		UpdateCommandBuffers(NextFrameIndex);
//		Renderer.RunCommandBuffers.clear();
//		Renderer.RunCommandBuffers.emplace_back(Renderer.SecondaryCommandBuffers[NextFrameIndex]);
//		Renderer.RunCommandBuffers.emplace_back(guiDebugger.GetCommandBuffers(NextFrameIndex));
//		Renderer.EndFrame(Window.GetWindowPtr(), NextFrameIndex);
//	}
//
//	void UpdateMouse()
//	{
//		if (glfwGetMouseButton(Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
//		{
//			glfwGetCursorPos(Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
//			if (firstMouse)
//			{
//				lastX = MouseXPos;
//				lastY = MouseYPos;
//				firstMouse = false;
//			}
//
//			float xoffset = MouseXPos - lastX;
//			float yoffset = lastY - MouseYPos;
//
//			lastX = MouseXPos;
//			lastY = MouseYPos;
//
//			camera.UpdateMouse(xoffset, yoffset);
//		}
//		else
//		{
//			firstMouse = true;
//		}
//	}
//
//	void UpdateKeyboard()
//	{
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
//		{
//			camera.UpdateKeyboard(FORWARD, deltaTime);
//		}
//		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
//		{
//			camera.UpdateKeyboard(BACKWARD, deltaTime);
//		}
//		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
//		{
//			camera.UpdateKeyboard(LEFT, deltaTime);
//		}
//		if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
//		{
//			camera.UpdateKeyboard(RIGHT, deltaTime);
//		}
//	}
//};
//
//int main() {
//	HelloTriangleApplication app;
//
//	try {
//		app.run();
//	}
//	catch (const std::exception & e) {
//		std::cerr << e.what() << std::endl;
//		return EXIT_FAILURE;
//	}
//
//	return EXIT_SUCCESS;
//}
//
