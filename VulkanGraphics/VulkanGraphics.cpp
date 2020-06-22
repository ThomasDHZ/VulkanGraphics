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

	//Ambiant = AmbientLight(renderer, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	//modelLoader = ModelLoader(renderer, FileSystem::getPath("VulkanGraphics/Models/Nanosuit/nanosuit.obj"));
	//modelLoader.ModelMeshList[0].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[0].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[0].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[1].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[1].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[2].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[2].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[3].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[4].TextureList.emplace_back(texture);
	//modelLoader.ModelMeshList[4].TextureList.emplace_back(texture);

	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	maps.DiffuseMap = Texture2D(renderer, "texture/zxc_diffuseOriginal.bmp");
	maps.NormalMap = Texture2D(renderer, "texture/zxc_normal.bmp");
	maps.DisplacementMap = Texture2D(renderer, "texture/zxc_height.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/SparkManSpec2048.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/temp.bmp");
	maps.CubeMap = CubeMapTexture(renderer, layout);

	Skybox = SkyBox(renderer, maps.CubeMap);

	InitializeGUIDebugger();
	MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));
	//MeshList.emplace_back(Mesh(renderer, vertices, indices, maps));

	//ModelList.emplace_back(Model(renderer, modelLoader.GetModelMeshs()));

	glm::vec3 pointLightPositions[] = {
glm::vec3(0.7f,  0.2f,  2.0f),
glm::vec3(2.3f, -3.3f, -4.0f),
glm::vec3(-4.0f,  2.0f, -12.0f),
glm::vec3(0.0f,  0.0f, -3.0f)
	};

	LightPos = glm::vec3(0.5f, 1.0f, 0.3f);

	DirectionalLightBuffer lighter;
	lighter.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lighter.Ambient = glm::vec3(0.0, 0.0, 0.0);
	lighter.Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	lighter.Specular = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLightBuffer light1;
	light1.Position = pointLightPositions[0];
	light1.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light1.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light1.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.Constant = 1.0f;
	light1.Linear = 0.09f;
	light1.Quadratic = 0.032;

	PointLightBuffer light2;
	light2.Position = pointLightPositions[1];
	light2.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light2.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light2.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light2.Constant = 1.0f;
	light2.Linear = 0.09f;
	light2.Quadratic = 0.032;

	PointLightBuffer light3;
	light3.Position = pointLightPositions[2];
	light3.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light3.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light3.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light3.Constant = 1.0f;
	light3.Linear = 0.09f;
	light3.Quadratic = 0.032;
	
	PointLightBuffer light4;
	light4.Position = pointLightPositions[3];
	light4.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light4.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light4.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light4.Constant = 1.0f;
	light4.Linear = 0.09f;
	light4.Quadratic = 0.032;

	SpotLightBuffer spotLight = {};
	spotLight.Position = camera.Position;
	spotLight.Direction = camera.Front;
	spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Constant = 1.0f;
	spotLight.Linear = 0.09f;
	spotLight.Quadratic = 0.032f;
	spotLight.CutOff = glm::cos(glm::radians(12.5f));
	spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));

	lightManager.DirectionalLightList.emplace_back(DirectionalLight(renderer, lighter));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light1));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light2));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light3));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light4));
	lightManager.SpotlightList.emplace_back(SpotLight(renderer, spotLight));
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
	//modelLoader.CleanTextureMemory(renderer);
	for (auto mesh : MeshList)
	{
		mesh.Destroy(renderer);
	}
	lightManager.Destroy(renderer);
	//debugLightMesh.Destroy(renderer);
	//for (auto model : ModelList)
	//{
	//	model.Destroy(renderer);
	//}
	//Ambiant.Destroy(renderer);
	
	Skybox.Destory(renderer);

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

void VulkanGraphics::Update(uint32_t DrawFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	glm::vec3 cubePositions[] = {
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


	Material material = {};
	material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.Shininess = 32.0f;


	glm::vec3 lightPositions[] = {
	glm::vec3(-3.0f, 0.0f, 0.0f),
	glm::vec3(-1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f)
	};
	glm::vec3 lightColors[] = {
		glm::vec3(0.25f,0.25f,0.25f),
		glm::vec3(0.50f,0.50f,0.50f),
		glm::vec3(0.75f,0.75f,0.75f),
		glm::vec3(1.00f,1.00f,1.00f)
	};

	viewing.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	viewing.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
	viewing.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
	viewing.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
	viewing.material.Shininess = 256;
	viewing.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	viewing.pointLight.Ambient = glm::vec3(0.005f);
	viewing.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	viewing.pointLight.Specular = glm::vec3(0.3f);
	viewing.pointLight.Constant = 1.0f;
	viewing.pointLight.Linear = 0.09f;
	viewing.pointLight.Quadratic = 0.032f;
	viewing.spotLight.Position = camera.Position;
	viewing.spotLight.Direction = camera.Front;
	viewing.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	viewing.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	viewing.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	viewing.spotLight.Constant = 1.0f;
	viewing.spotLight.Linear = 0.09f;
	viewing.spotLight.Quadratic = 0.032f;
	viewing.spotLight.CutOff = glm::cos(glm::radians(12.5f));
	viewing.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
	viewing.LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
	viewing.viewPos = camera.Position;



	for (unsigned int i = 0; i < 1; i++)
	{
		float angle = 20.0f * i;

		PositionMatrix ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::rotate(ubo.model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
		ubo.proj[1][1] *= -1;
		ubo.lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
		ubo.viewPos = camera.Position;

		viewing.model = ubo.model;
		MeshList[i].UpdateUniformBuffer(renderer, ubo, viewing);
	}


	lightManager.Update(renderer, camera);

	SkyBoxPositionMatrix skyUbo = {};
	skyUbo.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyUbo.projection = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
	skyUbo.projection[1][1] *= -1;

	Skybox.UpdateUniformBuffer(renderer, skyUbo);
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

	guiDebugger.UpdateCommandBuffers(DrawFrame, *GetRenderPass(renderer), renderer.SwapChainFramebuffers[DrawFrame]);
}

void VulkanGraphics::Draw()
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

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		mouse.Update(Window.GetWindowPtr(), camera, renderer.Settings);
		keyboard.Update(Window.GetWindowPtr(), camera, renderer.Settings);
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{

			ImGui::Begin("Settings");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
			ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
			ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
			ImGui::SliderFloat("Displace", &viewing.Height, 0.0f, 1.0f);
			//ImGui::Image(&descriptorWrites, ImVec2(100.0f, 100.0f));

			ImGui::End();

			lightManager.UpdateLights();
		}

		ImGui::Render();
		Draw();
	}
}