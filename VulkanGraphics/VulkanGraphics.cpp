#include "VulkanGraphics.h"
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = Renderer(Window.GetWindowPtr());

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	modelLoader = ModelLoader(renderer, FileSystem::getPath("VulkanGraphics/Models/suzanne.obj"));

	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	maps.DiffuseMap = Texture2D(renderer, "texture/zxc_diffuseOriginal.bmp");
	maps.NormalMap = Texture2D(renderer, "texture/Temp.bmp");
	maps.DisplacementMap = Texture2D(renderer, "texture/Temp.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/Temp.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/Temp.bmp");
	maps.CubeMap = CubeMapTexture(renderer, layout);

	Skybox = SkyBox(renderer, maps.CubeMap);
	MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	ModelList.emplace_back(Model(renderer, modelLoader.GetModelMeshs()));

	LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(*GetDevice(renderer));

	maps.DiffuseMap.Destroy(renderer);
	maps.NormalMap.Destroy(renderer);
	maps.DisplacementMap.Destroy(renderer);
	maps.SpecularMap.Destroy(renderer);
	maps.AlphaMap.Destroy(renderer);
	maps.CubeMap.Destroy(renderer);

	for (auto mesh : MeshList)
	{
		mesh.Destroy(renderer);
	}
	lightManager.Destroy(renderer);
	
	Skybox.Destory(renderer);

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::Update(uint32_t DrawFrame)
{

	light.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	light.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
	light.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
	light.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
	light.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	light.pointLight.Ambient = glm::vec3(0.005f);
	light.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light.pointLight.Specular = glm::vec3(0.3f);
	light.pointLight.Constant = 1.0f;
	light.pointLight.Linear = 0.09f;
	light.pointLight.Quadratic = 0.032f;
	light.spotLight.Position = camera.Position;
	light.spotLight.Direction = camera.Front;
	light.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	light.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Constant = 1.0f;
	light.spotLight.Linear = 0.09f;
	light.spotLight.Quadratic = 0.032f;
	light.spotLight.CutOff = glm::cos(glm::radians(12.5f));
	light.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
	light.viewPos = camera.Position;

	for (auto mesh : MeshList)
	{
		mesh.Update(renderer, camera, light);
	}
	for (auto model : ModelList)
	{
		model.UpdateUniformBuffer(renderer, camera, light);
	}
	Skybox.UpdateUniformBuffer(renderer, camera);
}

void VulkanGraphics::UpdateCommandBuffers(uint32_t DrawFrame)
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
			for (auto mesh : MeshList)
			{
				mesh.Draw(renderer, i);
			}
			for (auto model : ModelList)
			{
				model.Draw(renderer, i);
			}
			if (renderer.Settings.ShowDebugLightMesh)
			{
				lightManager.DrawDebugMesh(renderer, i);
			}
			if (renderer.Settings.ShowSkyBox)
			{
				Skybox.Draw(renderer, i);
			}
			if (vkEndCommandBuffer(renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		renderer.UpdateCommandBuffers = false;
	}
}

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		if (CompareVulkanSettings != renderer.Settings)
		{
			CompareVulkanSettings = renderer.Settings;
			renderer.UpdateSwapChain(Window.GetWindowPtr());
		}

		Window.Update();
		mouse.Update(Window.GetWindowPtr(), camera, renderer.Settings);
		keyboard.Update(Window.GetWindowPtr(), camera, renderer.Settings);
		Update(renderer.DrawFrame);
		UpdateCommandBuffers(renderer.DrawFrame);



		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{

			ImGui::Begin("Settings");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
			ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
			ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
			ImGui::End();

			MeshList[0].UpdateGUI();
		}
	
		ImGui::Render();
		renderer.Draw(Window.GetWindowPtr());
	}
}