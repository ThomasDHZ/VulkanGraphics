#include "VulkanGraphics2D.h"


VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = VulkanRenderer(Window.GetWindowPtr());
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	texture = Texture2D(renderer, "texture/container2.png");
	texture2 = Texture2D(renderer, "texture/container2_specular.png");
	std::vector<Texture2D> textureList = { texture, texture2 };


	InitializeGUIDebugger();
	for (int x = 0; x < 400; x++)
	{
		MeshList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	}



	glm::vec3 pointLightPositions[] = {
glm::vec3(0.7f,  0.2f,  2.0f),
glm::vec3(2.3f, -3.3f, -4.0f),
glm::vec3(-4.0f,  2.0f, -12.0f),
glm::vec3(0.0f,  0.0f, -3.0f)
	};

	LightPos = glm::vec3(0.0f, 0.0f, 2.0f);

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
	VkPhysicalDeviceRayTracingPropertiesNV AL;

	PointLightBuffer light4;
	light4.Position = pointLightPositions[3];
	light4.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light4.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light4.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light4.Constant = 1.0f;
	light4.Linear = 0.09f;
	light4.Quadratic = 0.032;

	SpotLightBuffer spotLight = {};
	spotLight.Position = camera.GetCameraPos();
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

VulkanGraphics2D::~VulkanGraphics2D()
{
	vkDeviceWaitIdle(*GetDevice(renderer));

	for (auto mesh : MeshList)
	{
		mesh.Destroy(renderer);
	}
	lightManager.Destroy(renderer);
	texture.Destroy(renderer);

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

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(2.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 2.0f, 0.0f),
	glm::vec3(1.0f, 2.0f, 0.0f),
	glm::vec3(2.0f, 2.0f, 0.0f),
	glm::vec3(3.0f, 0.0f, 0.0f)
	};


	Material material = {};
	material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.Shininess = 32.0f;

	MeshProp viewing = {};
	viewing.directionalLight = lightManager.DirectionalLightList[0].GetSettings();
	viewing.pointLight[0] = lightManager.PointLightList[0].GetSettings();
	viewing.pointLight[1] = lightManager.PointLightList[1].GetSettings();
	viewing.pointLight[2] = lightManager.PointLightList[2].GetSettings();
	viewing.pointLight[3] = lightManager.PointLightList[3].GetSettings();
	viewing.spotLight = lightManager.SpotlightList[0].GetSettings();
	viewing.material = material;
	viewing.viewPos = camera.GetCameraPos();


	for (unsigned int x = 0; x < 20; x++)
	{
		for (unsigned int i = 0; i < 20; i++)
		{
			float angle = 20.0f * i;

			PositionMatrix ubo{};
			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, glm::vec3(i, x, 0.0f));
			ubo.view = camera.GetViewMatrix();
			ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
			ubo.proj[1][1] *= -1;

			MeshList[x + (i * 20)].UpdateUniformBuffer(renderer, ubo, viewing);
		}
	}


	lightManager.Update(renderer, camera);
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
			for (auto mesh : MeshList)
			{
				mesh.Draw(renderer, i);
			}
			if (renderer.Settings.ShowDebugLightMeshs)
			{
				lightManager.DrawDebugMesh(renderer, i);
			}
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
		mouse.Update(Window.GetWindowPtr(), camera);
		keyboard.Update(Window.GetWindowPtr(), camera);
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::Begin("Settings");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
			ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMeshs);
			ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
			ImGui::SliderFloat3("Camera", camera.GetCameraPosPtr(), -10.0f, 10.0f);
			ImGui::End();

			lightManager.UpdateLights();
		}

		ImGui::Render();
		Draw();
	}
}