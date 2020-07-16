#include "VulkanGraphics.h"
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "NewTexture2D.h"

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = Renderer(Window.GetWindowPtr());

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));

	modelLoader = ModelLoader(renderer, FileSystem::getPath("VulkanGraphics/Models/monitor.obj"));
	texture = NewTexture2D(renderer.Device, renderer.PhysicalDevice, renderer.MainCommandPool, renderer.GraphicsQueue, "texture/texture.jpg");

	mesh = Mesh2(renderer.Device, renderer.PhysicalDevice, renderer.MainCommandPool, renderer.GraphicsQueue, renderer.SwapChain.SwapChainImages, modelLoader.GetModelMeshs()[0].VertexList, modelLoader.GetModelMeshs()[0].IndexList, texture, renderer.forwardRenderer.DescriptorSetLayout);
	quad = Mesh2(renderer.Device, renderer.PhysicalDevice, renderer.MainCommandPool, renderer.GraphicsQueue, renderer.SwapChain.SwapChainImages, quadvertices, quadindices, renderer.textureRenderer.ColorTexture, renderer.textureRenderer.DescriptorSetLayout);


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

	//for (auto mesh : MeshList)
	//{
	//	//mesh.Destroy(renderer);
	//}
	lightManager.Destroy(renderer);
	
	Skybox.Destory(renderer);

	//framebuffer.Destory(renderer.Device);

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::Update(uint32_t DrawFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

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

	{
		UniformBufferObject ubo{};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(renderer.Device, mesh.UniformBuffersMemory[DrawFrame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(renderer.Device, mesh.UniformBuffersMemory[DrawFrame]);
	}
	{
		UniformBufferObject ubo{};
		ubo.model = glm::mat4(1.0f);
		ubo.model = glm::translate(ubo.model, glm::vec3(-2.5f, 0.0f, 0.0f));
		// ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(-0.5f, 0.5f, 0.5f));
		ubo.view = camera.GetViewMatrix();
		ubo.proj = glm::perspective(glm::radians(camera.Zoom), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 10000.0f);
		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(renderer.Device, quad.UniformBuffersMemory[DrawFrame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(renderer.Device, quad.UniformBuffersMemory[DrawFrame]);
	}


}

void VulkanGraphics::UpdateCommandBuffers(uint32_t DrawFrame)
{
	if (renderer.UpdateCommandBuffers)
	{
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


		}

		ImGui::Render();
		auto DrawFrame = renderer.DrawStart(Window.GetWindowPtr());

		VkCommandBufferBeginInfo CommandBufferInfo = {};
		CommandBufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;


		if (vkBeginCommandBuffer(renderer.MainCommandBuffer[DrawFrame], &CommandBufferInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		{
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 1.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderer.textureRenderer.RenderPass;
			renderPassInfo.framebuffer = renderer.textureRenderer.swapChainFramebuffers[DrawFrame];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(renderer.MainCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			{
				VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindPipeline(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.textureRenderer.RendererPipeline);
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(renderer.MainCommandBuffer[DrawFrame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.textureRenderer.RendererLayout, 0, 1, &mesh.DescriptorSets[DrawFrame], 0, nullptr);
				vkCmdDrawIndexed(renderer.MainCommandBuffer[DrawFrame], static_cast<uint32_t>(modelLoader.GetModelMeshs()[0].IndexList.size()), 1, 0, 0, 0);
			}
			vkCmdEndRenderPass(renderer.MainCommandBuffer[DrawFrame]);
		}
		{
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.2f, 0.2f, 0.2f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderer.forwardRenderer.RenderPass;
			renderPassInfo.framebuffer = renderer.forwardRenderer.swapChainFramebuffers[DrawFrame];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = renderer.SwapChain.GetSwapChainResolution();
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(renderer.MainCommandBuffer[DrawFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			{
				VkBuffer vertexBuffers[] = { mesh.VertexBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindPipeline(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.forwardRenderer.RendererPipeline);
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(renderer.MainCommandBuffer[DrawFrame], mesh.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.forwardRenderer.RendererLayout, 0, 1, &mesh.DescriptorSets[DrawFrame], 0, nullptr);
				vkCmdDrawIndexed(renderer.MainCommandBuffer[DrawFrame], static_cast<uint32_t>(modelLoader.GetModelMeshs()[0].IndexList.size()), 1, 0, 0, 0);
			}
			{
				VkBuffer vertexBuffers[] = { quad.VertexBuffer };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(renderer.MainCommandBuffer[DrawFrame], 0, 1, vertexBuffers, offsets);
				vkCmdBindIndexBuffer(renderer.MainCommandBuffer[DrawFrame], quad.IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
				vkCmdBindDescriptorSets(renderer.MainCommandBuffer[DrawFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.forwardRenderer.RendererLayout, 0, 1, &quad.DescriptorSets[DrawFrame], 0, nullptr);
				vkCmdDrawIndexed(renderer.MainCommandBuffer[DrawFrame], static_cast<uint32_t>(quadindices.size()), 1, 0, 0, 0);
			}
			vkCmdEndRenderPass(renderer.MainCommandBuffer[DrawFrame]);
		}

		if (vkEndCommandBuffer(renderer.MainCommandBuffer[DrawFrame]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}

		renderer.DrawEnd(Window.GetWindowPtr(), DrawFrame);
	}
}