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
	renderer = VulkanRenderer(Window.GetWindowPtr());
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	texture = Texture2D(renderer, "texture/container2.png");
	std::vector<Texture2D> textureList = { texture, texture };

	Ambiant = AmbientLight(renderer, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	//modelLoader = ModelLoader(renderer, FileSystem::getPath("VulkanGraphics/Models/Nanosuit/nanosuit.obj"));

	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";
	SkyboxTexture = CubeMapTexture(renderer, layout);

	Skybox = SkyBox(renderer, SkyboxTexture);

	InitializeGUIDebugger();
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	//ModelList.emplace_back(Model(renderer, modelLoader.GetModelMeshs()));

	lighter.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	lighter.Position = glm::vec3(1.2f, 1.0f, 2.0f);
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(*GetDevice(renderer));

	//modelLoader.CleanTextureMemory(renderer);
	for (auto mesh : MeshList)
	{
		mesh.Destroy(renderer);
	}
	//for (auto model : ModelList)
	//{
	//	model.Destroy(renderer);
	//}
	Skybox.Destory(renderer);
	Ambiant.Destroy(renderer);
	texture.Destroy(renderer);
	SkyboxTexture.Destroy(renderer);

	guiDebugger.ShutDown(*GetDevice(renderer));
	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::InitializeGUIDebugger()
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

void VulkanGraphics::Update(uint32_t NextFrameIndex)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	lighter.CameraPosition = glm::vec3(camera.GetCameraPos());

	for (int x = 0; x < MeshList.size(); x++)
	{
		MeshList[x].MeshPosition = cubePositions[x];
		UniformBufferObject ubo = {};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::translate(ubo.model, cubePositions[x]);
		ubo.model = glm::rotate(ubo.model, glm::radians(time * 20.0f), MeshList[x].MeshRotate);
		ubo.model = glm::scale(ubo.model, MeshList[x].MeshScale);
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
		ubo.proj[1][1] *= -1;

		MeshList[x].UpdateUniformBuffer(renderer, ubo, Ambiant.GetLightSettings(), lighter, NextFrameIndex);
	}

	//for (int x = 0; x < ModelList.size(); x++)
	//{
	//	ModelList[x].ModelPosition = cubePositions[x];
	//	UniformBufferObject ubo = {};
	//	ubo.model = glm::mat4(1.0f);
	//	ubo.model = glm::translate(ubo.model, ModelList[x].ModelPosition);
	//	ubo.model = glm::rotate(ubo.model, glm::radians(time * 20.0f), ModelList[x].ModelRotate);
	//	ubo.model = glm::scale(ubo.model, ModelList[x].ModelScale);
	//	ubo.view = camera.GetViewMatrix();
	//	ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	//	ubo.proj[1][1] *= -1;

	//	ModelList[x].UpdateUniformBuffer(renderer, ubo, Ambiant.GetLightSettings(), lighter, NextFrameIndex);
	//}

	SkyBoxUniformBufferObject skyUbo = {};
	skyUbo.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyUbo.projection = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	skyUbo.projection[1][1] *= -1;

	Skybox.UpdateUniformBuffer(renderer, skyUbo, NextFrameIndex);
}

void VulkanGraphics::UpdateCommandBuffers(uint32_t NextFrameIndex)
{
	if (renderer.UpdateCommandBuffers)
	{
		for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
		{
			VkCommandBufferInheritanceInfo InheritanceInfo = {};
			InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
			InheritanceInfo.renderPass = *GetRenderPass(renderer);
			InheritanceInfo.framebuffer = renderer.swapChainFramebuffers[i];

			VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
			BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

			vkBeginCommandBuffer(renderer.SubCommandBuffers[i], &BeginSecondaryCommandBuffer);
			for (auto mesh : MeshList)
			{
				mesh.Draw(renderer, i);
			}
	/*		for (auto model : ModelList)
			{
				model.Draw(renderer, i);
			}*/
			if (renderer.Settings.ShowSkyBox)
			{
				Skybox.Draw(renderer, i);
			}
			if (vkEndCommandBuffer(renderer.SubCommandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		renderer.UpdateCommandBuffers = false;
	}

	guiDebugger.UpdateCommandBuffers(NextFrameIndex, *GetRenderPass(renderer), renderer.swapChainFramebuffers[NextFrameIndex]);
}

void VulkanGraphics::Draw()
{
	if (CompareVulkanSettings != renderer.Settings)
	{
		CompareVulkanSettings = renderer.Settings;
		renderer.UpdateSwapChain(Window.GetWindowPtr());
	}

	auto NextFrameIndex = renderer.StartFrame(Window.GetWindowPtr());
	Update(NextFrameIndex);
	UpdateCommandBuffers(NextFrameIndex);
	renderer.RunCommandBuffers.clear();
	renderer.RunCommandBuffers.emplace_back(renderer.SubCommandBuffers[NextFrameIndex]);
	renderer.RunCommandBuffers.emplace_back(guiDebugger.GetCommandBuffers(NextFrameIndex));
	renderer.EndFrame(Window.GetWindowPtr(), NextFrameIndex);
}

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		UpdateMouse();
		UpdateKeyboard();
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Hello, world!");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
			ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
			ImGui::SliderFloat("float", Ambiant.GetColorStrengthPtr(), 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)Ambiant.GetColorPtr()); // Edit 3 floats representing a color
			ImGui::ColorEdit3("Position", (float*)&lighter.Position); // Edit 3 floats representing a color
			ImGui::ColorEdit3("Color", (float*)&lighter.Color); // Edit 3 floats representing a color
			ImGui::ColorEdit3("CameraPosition", (float*)&lighter.CameraPosition); // Edit 3 floats representing a color
			ImGui::End();
		}
		ImGui::Render();
		Draw();
	}
}

void VulkanGraphics::UpdateMouse()
{
	if (glfwGetMouseButton(Window.GetWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwGetCursorPos(Window.GetWindowPtr(), &MouseXPos, &MouseYPos);
		if (firstMouse)
		{
			lastX = MouseXPos;
			lastY = MouseYPos;
			firstMouse = false;
		}

		float xoffset = MouseXPos - lastX;
		float yoffset = lastY - MouseYPos;

		lastX = MouseXPos;
		lastY = MouseYPos;

		camera.UpdateMouse(xoffset, yoffset);
	}
	else
	{
		firstMouse = true;
	}
}

void VulkanGraphics::UpdateKeyboard()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.UpdateKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.UpdateKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.UpdateKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.UpdateKeyboard(RIGHT, deltaTime);
	}
}
