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
	modelLoader = ModelLoader(renderer, FileSystem::getPath("VulkanGraphics/Models/monitor.obj"));

	framebuffer = FrameBufferMesh(renderer.Device, renderer.PhysicalDevice, renderer.MainCommandPool, renderer.GraphicsQueue, renderer.SwapChain.GetSwapChainResolution(), renderer.RenderPass, renderer.HDRColorAttachment, renderer.DepthAttachment, renderer.FrameBufferPipeline.ShaderPipelineDescriptorLayout, renderer.SwapChain.GetSwapChainImageCount());
	rendereredTexture = EngineRenderedTexture(renderer, VK_FORMAT_R32G32B32A32_SFLOAT, renderer.SwapChain.GetSwapChainResolution());

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

	framebuffer.Destory(renderer.Device);

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
		framebuffer.RecreateSwapChainStage(renderer.Device, renderer.SwapChain.GetSwapChainResolution(), renderer.RenderPass, renderer.HDRColorAttachment, renderer.DepthAttachment, renderer.FrameBufferPipeline.ShaderPipelineDescriptorLayout, renderer.SwapChain.GetSwapChainImageCount());

		//for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
		//{
		//	VkCommandBufferInheritanceInfo InheritanceInfo = {};
		//	InheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		//	InheritanceInfo.renderPass = *GetRenderPass(renderer);
		//	InheritanceInfo.framebuffer = renderer.SwapChainFramebuffers[i];

		//	VkCommandBufferBeginInfo BeginSecondaryCommandBuffer = {};
		//	BeginSecondaryCommandBuffer.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//	BeginSecondaryCommandBuffer.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT | VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		//	BeginSecondaryCommandBuffer.pInheritanceInfo = &InheritanceInfo;

		//	vkBeginCommandBuffer(renderer.SecondaryCommandBuffers[i], &BeginSecondaryCommandBuffer);
		//	for (auto mesh : MeshList)
		//	{
		//		mesh.Draw(renderer, i);
		//	}
		//	for (auto model : ModelList)
		//	{
		//		model.Draw(renderer, i);
		//	}
		//	if (renderer.Settings.ShowDebugLightMesh)
		//	{
		//		lightManager.DrawDebugMesh(renderer, i);
		//	}
		//	if (renderer.Settings.ShowSkyBox)
		//	{
		//		Skybox.Draw(renderer, i);
		//	}
		//	if (vkEndCommandBuffer(renderer.SecondaryCommandBuffers[i]) != VK_SUCCESS) {
		//		throw std::runtime_error("failed to record command buffer!");
		//	}
		//}

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

			//QuadMesh.UpdateGUI();
		}
	
		ImGui::Render();
		auto DrawFrame = renderer.DrawStart(Window.GetWindowPtr());

		VkCommandBufferBeginInfo CommandBufferInfo = {};
		CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(renderer.MainCommandBuffer[DrawFrame], &CommandBufferInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		{
			std::array<VkClearValue, 3> clearValues = {};
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[2].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderer.RenderPass;
			renderPassInfo.framebuffer = renderer.SwapChainFramebuffers[DrawFrame];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(renderer.MainCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				VkBuffer vertexBuffers[] = { MeshList[0].vertexBuffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(renderer));
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &MeshList[0].descriptorSets[DrawFrame], 0, nullptr);
				if (MeshList[0].IndiceSize == 0)
				{
					vkCmdDraw(renderer.MainCommandBuffer[DrawFrame], MeshList[0].VertexSize, 1, 0, 0);
				}
				else
				{
					vkCmdBindIndexBuffer(renderer.MainCommandBuffer[DrawFrame], MeshList[0].indexBuffer, 0, VK_INDEX_TYPE_UINT16);
					vkCmdDrawIndexed(renderer.MainCommandBuffer[DrawFrame], static_cast<uint32_t>(MeshList[0].IndiceSize), 1, 0, 0, 0);
				}
			}
			{
				VkBuffer vertexBuffers[] = { ModelList[0].MeshList[0].vertexBuffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipeline(renderer));
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetShaderPipelineLayout(renderer), 0, 1, &ModelList[0].MeshList[0].descriptorSets[DrawFrame], 0, nullptr);
				if (ModelList[0].MeshList[0].IndiceSize == 0)
				{
					vkCmdDraw(renderer.MainCommandBuffer[DrawFrame], ModelList[0].MeshList[0].VertexSize, 1, 0, 0);
				}
				else
				{
					vkCmdBindIndexBuffer(renderer.MainCommandBuffer[DrawFrame], ModelList[0].MeshList[0].indexBuffer, 0, VK_INDEX_TYPE_UINT16);
					vkCmdDrawIndexed(renderer.MainCommandBuffer[DrawFrame], static_cast<uint32_t>(ModelList[0].MeshList[0].IndiceSize), 1, 0, 0, 0);
				}
			}
			{
				VkBuffer vertexBuffers[] = { Skybox.vertexBuffer };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindPipeline(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipeline(renderer));
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, *GetSkyboxShaderPipelineLayout(renderer), 0, 1, &Skybox.descriptorSets[DrawFrame], 0, nullptr);
				vkCmdDraw(renderer.MainCommandBuffer[DrawFrame], Skybox.VertexSize, 1, 0, 0);
			}
		}
		vkCmdNextSubpass(renderer.MainCommandBuffer[DrawFrame], VK_SUBPASS_CONTENTS_INLINE);
		framebuffer.Draw(renderer.FrameBufferPipeline, renderer.MainCommandBuffer[DrawFrame], DrawFrame);
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderer.MainCommandBuffer[DrawFrame]);
		vkCmdEndRenderPass(renderer.MainCommandBuffer[DrawFrame]);
		vkEndCommandBuffer(renderer.MainCommandBuffer[DrawFrame]);

		renderer.DrawEnd(Window.GetWindowPtr(), DrawFrame);
	}
}