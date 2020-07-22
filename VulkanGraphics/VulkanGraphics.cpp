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


	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	//maps.DiffuseMap = Texture2D(renderer, "texture/zxc_diffuseOriginal.bmp");
	//maps.NormalMap = Texture2D(renderer, "texture/Temp.bmp");
	//maps.DisplacementMap = Texture2D(renderer, "texture/Temp.bmp");
	//maps.SpecularMap = Texture2D(renderer, "texture/Temp.bmp");
	//maps.AlphaMap = Texture2D(renderer, "texture/Temp.bmp");
	//maps.CubeMap = CubeMapTexture(renderer, layout);

	newtexturebox = CubeMapTexture(*renderer.GetVulkanRendererBase(), layout);
	newtexture = Texture2D(*renderer.GetVulkanRendererBase(), "texture/skybox/left.jpg");
	newtexture2 = Texture2D(*renderer.GetVulkanRendererBase(), "texture/wood.png");
	Skybox = SkyBoxMesh(*renderer.GetVulkanRendererBase(), renderer.forwardRenderer.skyboxPipeline.ShaderPipelineDescriptorLayout, newtexturebox);
	modelLoader = ModelLoader(*renderer.GetVulkanRendererBase(), FileSystem::getPath("VulkanGraphics/Models/gobber/GoblinX.obj"));
	MeshList.emplace_back(Mesh2(*renderer.GetVulkanRendererBase(), modelLoader.GetModelMeshs()[0].VertexList, modelLoader.GetModelMeshs()[0].IndexList, newtexture2, renderer.forwardRenderer.DescriptorSetLayout));
	modelLoader = ModelLoader(*renderer.GetVulkanRendererBase(), FileSystem::getPath("VulkanGraphics/Models/suzanne.obj"));
	MeshList.emplace_back(Mesh2(*renderer.GetVulkanRendererBase(), modelLoader.GetModelMeshs()[0].VertexList, modelLoader.GetModelMeshs()[0].IndexList, newtexture2, renderer.forwardRenderer.DescriptorSetLayout));
	MeshList.emplace_back(Mesh2(*renderer.GetVulkanRendererBase(), vertices, indices, newtexture2, renderer.forwardRenderer.DescriptorSetLayout));
	MeshList.emplace_back(Mesh2(*renderer.GetVulkanRendererBase(), quadvertices, quadindices, renderer.textureRenderer.ColorTexture, renderer.forwardRenderer.DescriptorSetLayout));
	frameBuffer = FrameBufferMesh(*renderer.GetVulkanRendererBase(), renderer.textureRenderer.ColorTexture, renderer.forwardRenderer.frameBufferPipeline.ShaderPipelineDescriptorLayout);

	//ModelList.emplace_back(Model(renderer, modelLoader.GetModelMeshs()));

	LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	//maps.DiffuseMap.Destroy(renderer);
	//maps.NormalMap.Destroy(renderer);
	//maps.DisplacementMap.Destroy(renderer);
	//maps.SpecularMap.Destroy(renderer);
	//maps.AlphaMap.Destroy(renderer);
	//maps.EmissionMap.Destroy(renderer);
	//maps.CubeMap.Destroy(renderer);


	newtexture.Delete(*renderer.GetVulkanRendererBase());

	for (auto mesh : MeshList)
	{
		mesh.Destory(*renderer.GetVulkanRendererBase());
	}
	//lightManager.Destroy(renderer);
	//
	//Skybox.Destory(renderer);

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::UpdateImGUI()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{

		ImGui::Begin("Settings");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Gamma", &frameBuffer.settings.Gamma, 0.0f, 10.0f);
		ImGui::SliderFloat("HDR Value", &frameBuffer.settings.HDRValue, 0.0f, 10.0f);
		ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
		ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
		ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
		ImGui::End();
	}

	ImGui::Render();
}

void VulkanGraphics::Update(uint32_t DrawFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	//light.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	//light.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
	//light.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
	//light.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
	//light.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	//light.pointLight.Ambient = glm::vec3(0.005f);
	//light.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	//light.pointLight.Specular = glm::vec3(0.3f);
	//light.pointLight.Constant = 1.0f;
	//light.pointLight.Linear = 0.09f;
	//light.pointLight.Quadratic = 0.032f;
	//light.spotLight.Position = camera.Position;
	//light.spotLight.Direction = camera.Front;
	//light.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	//light.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//light.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//light.spotLight.Constant = 1.0f;
	//light.spotLight.Linear = 0.09f;
	//light.spotLight.Quadratic = 0.032f;
	//light.spotLight.CutOff = glm::cos(glm::radians(12.5f));
	//light.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
	//light.viewPos = camera.Position;

	//MeshList[1].MeshPosition = glm::vec3(5.0f, 0.0f, 0.0f);
	//for (auto mesh : MeshList)
	//{
	//	mesh.Update(renderer, camera, light);
	//}
	//for (auto model : ModelList)
	//{
	//	model.UpdateUniformBuffer(renderer, camera, light);
	//}
	Skybox.UpdateUniformBuffer(renderer, camera);

	{
		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		MeshList[0].UpdateUniformBuffer(*renderer.GetVulkanRendererBase(), ubo);
	}
	{
		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::translate(ubo.model, glm::vec3(5.0f, 0.0f, 0.0f));
		ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		MeshList[1].UpdateUniformBuffer(*renderer.GetVulkanRendererBase(), ubo);
	}
	{
		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::translate(ubo.model, glm::vec3(0.0f, 0.0f, 0.0f));
		ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		MeshList[2].UpdateUniformBuffer(*renderer.GetVulkanRendererBase(), ubo);
	}
	{
		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::translate(ubo.model, glm::vec3(-2.5f, 0.0f, 0.0f));
		// ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(-0.5f, 0.5f, 0.5f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		MeshList[3].UpdateUniformBuffer(*renderer.GetVulkanRendererBase(), ubo);
	}
	{
		frameBuffer.UpdateUniformBuffer(*renderer.GetVulkanRendererBase());
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
		UpdateImGUI();
		Update(renderer.DrawFrame);
		renderer.Draw(Window.GetWindowPtr(), frameBuffer, Skybox, MeshList);
	}
}