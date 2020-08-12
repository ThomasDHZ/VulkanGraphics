#include "VulkanGraphics.h"
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ImGui/imgui_impl_vulkan.h"

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	gameManager = GameManager(*renderer.GetVulkanRendererBase());

	ActiveCamera = &renderer.camera;

	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	gameManager.textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), "texture/brick_diffuseOriginal.bmp");
	gameManager.textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), "texture/container2_specular.png");
	gameManager.textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), "texture/brick_normal.bmp");
	gameManager.textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), "texture/brick_height.bmp");
	gameManager.textureManager->LoadTexture(*renderer.GetVulkanRendererBase(), layout);
	//gameManager.textureManager->LoadTexture(renderer.textureRenderer.ColorTexture);
	//gameManager.textureManager->LoadTexture(renderer.textureRenderer.DepthTexture);

	modelLoader = ModelLoader(*renderer.GetVulkanRendererBase(), gameManager.textureManager, FileSystem::getPath("VulkanGraphics/Models/nano_textured/nanosuit.obj"));
//
	MeshList.emplace_back(Mesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, CalcVertex(), indices, 0, 1, 2, 3, 4, renderer.forwardRenderer.forwardRendereringPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	MeshList.emplace_back(Mesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, CalcVertex(), indices, 0, 1, 2, 3, 4, renderer.forwardRenderer.forwardRendereringPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	MeshList.emplace_back(Mesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, CalcVertex(), indices, 0, 1, 2, 3, 4, renderer.forwardRenderer.forwardRendereringPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	MeshList.emplace_back(Mesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, CalcVertex(), indices, 0, 1, 2, 3, 4, renderer.forwardRenderer.forwardRendereringPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	//MeshList.emplace_back(Mesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, quadvertices, quadindices, 5, 1, 2, 3, 4, renderer.forwardRenderer.forwardRendereringPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow));

	Skybox = SkyBoxMesh(*renderer.GetVulkanRendererBase(), gameManager.textureManager, renderer.forwardRenderer.skyboxPipeline.ShaderPipelineDescriptorLayout, 4);

	debugLightMesh = DebugLightMesh(*renderer.GetVulkanRendererBase(), quadvertices, quadindices, renderer.forwardRenderer.DebugLightPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderOnTexturePass);
	light.position = glm::vec3(0.5f, 1.0f, 0.3f);

}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	gameManager.textureManager->UnloadAllTextures(*renderer.GetVulkanRendererBase());
	for (auto mesh : MeshList)
	{
		mesh.Destory(*renderer.GetVulkanRendererBase());
	}
	debugLightMesh.Destory(renderer);
	Skybox.Destory(renderer);

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
		ImGui::SliderFloat("Gamma", &renderer.frameBuffer.settings.Gamma, 0.0f, 10.0f);
		ImGui::SliderFloat("HDR Value", &renderer.frameBuffer.settings.HDRValue, 0.0f, 10.0f);
		ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
		ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
		ImGui::Checkbox("Show SkyBox", &renderer.Settings.ShowSkyBox);
		//ImGui::Checkbox("Switch Camara", &SwatchCamara);
		ImGui::SliderFloat3("Light", &light.position.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("ambient", &light.ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("diffuse", &light.diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("specular", &light.specular.x, 0.0f, 1.0f);
		ImGui::Image(renderer.textureRenderer.ColorTexture.ImGuiDescriptorSet, ImVec2(400.0f, 255.0f));
		ImGui::Image(renderer.shadowRenderer.DepthTexture.ImGuiDescriptorSet, ImVec2(400.0f, 255.0f));
		ImGui::End();

		ImGui::Begin("MeshSettings");
		if (ImGui::TreeNode("MeshList"))
		{
			ImGui::Columns(2, "tree", true);

			for (int x = 0; x < MeshList.size(); x++)
			{
				bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Mesh%d", x);
				ImGui::NextColumn();
				ImGui::Text("Node contents");
				ImGui::NextColumn();
				if (open1)
				{
					ImGui::NextColumn();
					ImGui::SliderFloat("shininess", &MeshList[x].properites.material.shininess, 0.0, 255.0f);
					ImGui::SliderFloat("reflectivness", &MeshList[x].properites.material.reflectivness, 0.0f, 1.0f);
					ImGui::SliderFloat("heightScale", &MeshList[x].properites.heightScale, -1.0, 1.0f);
					ImGui::SliderFloat("Layers", &MeshList[x].properites.minLayers, 0.0, 50.0f);
					ImGui::SliderFloat("maxLayers", &MeshList[x].properites.maxLayers, 0.0, 5000.0f);
					ImGui::SliderInt("Diffuse", &MeshList[x].properites.UseDiffuseMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Specular", &MeshList[x].properites.UseSpecularMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Normal", &MeshList[x].properites.UseNormalMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Depth", &MeshList[x].properites.UseDepthMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Alpha", &MeshList[x].properites.UseAlphaMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Emission", &MeshList[x].properites.UseEmissionMapBit, 0.0, 1.0f);
					ImGui::Image(gameManager.textureManager->GetTexture(0).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTexture(1).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTexture(2).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTexture(3).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}

			ImGui::Columns(1);
			ImGui::TreePop();
		}
		ImGui::End();
		gameManager.textureManager->UpdateIMGUIVRAM();
	}

	ImGui::Render();
}

void VulkanGraphics::Update(uint32_t DrawFrame)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	light.viewPos = ActiveCamera->Position;

	MeshList[1].MeshPosition = glm::vec3(2.0f, 0.0f, 0.0f);
	MeshList[2].MeshPosition = glm::vec3(4.0f, 0.0f, 0.0f);
	MeshList[3].MeshPosition = glm::vec3(6.0f, 0.0f, 0.0f);
	//MeshList[4].MeshPosition = glm::vec3(8.0f, 0.0f, 0.0f);
	for (auto mesh : MeshList)
	{
		mesh.Update(renderer, *ActiveCamera, light);
	}
	//for (auto model : ModelList)
	//{
	//	model.UpdateUniformBuffer(renderer, camera, light);
	//}
	Skybox.UpdateUniformBuffer(renderer, renderer.camera);

	MeshColor color = {};
	color.Color = glm::vec3(1.0f, 1.0f, 1.0f);

	debugLightMesh.MeshPosition = light.position;
	debugLightMesh.MeshScale = glm::vec3(.1f, .1f, .1f);
	debugLightMesh.Update(renderer, *ActiveCamera, color);
	renderer.frameBuffer.UpdateUniformBuffer(renderer);
}

std::vector<Vertex> VulkanGraphics::CalcVertex()
{
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);


	return {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		{{pos1.x, pos1.y, pos1.z}, {nm.x, nm.y, nm.z}, {uv1.x, uv1.y}, {tangent1.x, tangent1.y, tangent1.z}, {bitangent1.x, bitangent1.y, bitangent1.z}},
		{{pos2.x, pos2.y, pos2.z}, {nm.x, nm.y, nm.z}, {uv2.x, uv2.y}, {tangent1.x, tangent1.y, tangent1.z}, {bitangent1.x, bitangent1.y, bitangent1.z}},
		{{pos3.x, pos3.y, pos3.z}, {nm.x, nm.y, nm.z}, {uv3.x, uv3.y}, {tangent1.x, tangent1.y, tangent1.z}, {bitangent1.x, bitangent1.y, bitangent1.z}},

		{{pos1.x, pos1.y, pos1.z}, {nm.x, nm.y, nm.z}, {uv1.x, uv1.y}, {tangent2.x, tangent2.y, tangent2.z}, {bitangent2.x, bitangent2.y, bitangent2.z}},
		{{pos3.x, pos3.y, pos3.z}, {nm.x, nm.y, nm.z}, {uv3.x, uv3.y}, {tangent2.x, tangent2.y, tangent2.z}, {bitangent2.x, bitangent2.y, bitangent2.z}},
		{{pos4.x, pos4.y, pos4.z}, {nm.x, nm.y, nm.z}, {uv4.x, uv4.y}, {tangent2.x, tangent2.y, tangent2.z}, {bitangent2.x, bitangent2.y, bitangent2.z}}
	};
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
		mouse.Update(Window.GetWindowPtr(), renderer.camera, renderer.Settings);
		keyboard.Update(Window.GetWindowPtr(), renderer.camera, renderer.Settings);
		UpdateImGUI();
		Update(renderer.DrawFrame);
		renderer.Draw(Window.GetWindowPtr(), MeshList, Skybox, debugLightMesh);
	}
}