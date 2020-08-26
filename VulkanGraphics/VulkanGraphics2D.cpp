#include "VulkanGraphics2D.h"
#include <FileSystem.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include "ImGui/imgui_impl_vulkan.h"
#include "Sprite.h"

VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
{

	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	gameManager = GameManager(renderer);

	MeshTextures SparkManTextures = {};
	SparkManTextures.DiffuseMap = "texture/SparkMan_diffuseOriginal.bmp";
	SparkManTextures.SpecularMap = "texture/container2_specular.png";
	SparkManTextures.NormalMap = "texture/SparkMan_normal.bmp";

	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "texture/SparkMan_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "texture/container2_specular.png";
	MegaManTextures.NormalMap = "texture/SparkMan_normal.bmp";

	light = Light(renderer, renderer.forwardRenderer.DebugLightPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderOnTexturePass, glm::vec3(0.0f));


	sprite.emplace_back(Sprite(renderer, gameManager.textureManager, 1.0f, 1.0f, MegaManTextures, glm::vec2(0.0f, 0.0f), SpriteType::SMegaMan, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	level = LevelSprite(renderer, gameManager.textureManager, SparkManTextures, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass);
}

VulkanGraphics2D::~VulkanGraphics2D()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	gameManager.textureManager->UnloadAllTextures(*renderer.GetVulkanRendererBase());
	//renderer.LevelMesh.Destory(renderer);
	renderer.debugLightMesh.Destory(renderer);

	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics2D::UpdateImGUI()
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
		ImGui::SliderFloat3("dLight", &light.light.dLight.direction.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("dambient", &light.light.dLight.ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("ddiffuse", &light.light.dLight.diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("dspecular", &light.light.dLight.specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pLight", &light.light.pLight.position.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("pambient", &light.light.pLight.ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pdiffuse", &light.light.pLight.diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pspecular", &light.light.pLight.specular.x, 0.0f, 1.0f);
		ImGui::End();

		ImGui::Begin("MeshSettings");
		if (ImGui::TreeNode("MeshList"))
		{
			ImGui::Columns(2, "tree", true);

		/*	for (int x = 0; x < renderer.MeshList.size(); x++)
			{
				bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Mesh%d", x);
				ImGui::NextColumn();
				ImGui::Text("Node contents");
				ImGui::NextColumn();
				if (open1)
				{
					ImGui::SliderFloat2("shininess", &renderer.MeshList[x].properites.UVOffset.x, 0.0, 1.0f);
					ImGui::SliderFloat("shininess", &renderer.MeshList[x].properites.material.shininess, 0.0, 255.0f);
					ImGui::SliderFloat("reflectivness", &renderer.MeshList[x].properites.material.reflectivness, 0.0f, 1.0f);
					ImGui::SliderFloat("heightScale", &renderer.MeshList[x].properites.heightScale, -1.0, 1.0f);
					ImGui::SliderFloat("Layers", &renderer.MeshList[x].properites.minLayers, 0.0, 50.0f);
					ImGui::SliderFloat("maxLayers", &renderer.MeshList[x].properites.maxLayers, 0.0, 5000.0f);
					ImGui::SliderInt("Diffuse", &renderer.MeshList[x].properites.UseDiffuseMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Specular", &renderer.MeshList[x].properites.UseSpecularMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Normal", &renderer.MeshList[x].properites.UseNormalMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Depth", &renderer.MeshList[x].properites.UseDepthMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Alpha", &renderer.MeshList[x].properites.UseAlphaMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Emission", &renderer.MeshList[x].properites.UseEmissionMapBit, 0.0, 1.0f);
					ImGui::SliderInt("Reflection", &renderer.MeshList[x].properites.UseReflectionMapBit, 0.0, 1.0f);
					ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.MeshList[x].DiffuseMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.MeshList[x].SpecularMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.MeshList[x].NormalMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.MeshList[x].DepthMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.MeshList[x].ReflectionMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}*/

			ImGui::Columns(1);
			ImGui::TreePop();
		}
		ImGui::End();
		gameManager.textureManager->UpdateIMGUIVRAM();
	}

	ImGui::Render();
}

void VulkanGraphics2D::Update(uint32_t DrawFrame)
{
	level.Update(renderer, renderer.OrthoCamera, light.light);
	for (auto mesh2 : sprite)
	{
		mesh2.Update(renderer, renderer.OrthoCamera, light.light);
	}
	light.Update(renderer, renderer.OrthoCamera);
	//const glm::vec3 BottomLeftVertex = renderer.LevelMesh[1].MeshPosition + renderer.LevelMesh[1].Vertexdata[0].Position;
	//const glm::vec3 BottomRightVertex = renderer.LevelMesh[1].MeshPosition + renderer.LevelMesh[1].Vertexdata[1].Position;
	//const glm::vec3 TopRightVertex = renderer.LevelMesh[1].MeshPosition + renderer.LevelMesh[1].Vertexdata[2].Position;
	//const glm::vec3 TopLeftVertex = renderer.LevelMesh[1].MeshPosition + renderer.LevelMesh[1].Vertexdata[3].Position;

	//const glm::vec3 BottomLeftVertex2 = renderer.LevelMesh[2].MeshPosition + renderer.LevelMesh[2].Vertexdata[0].Position;
	//const glm::vec3 BottomRightVertex2 = renderer.LevelMesh[2].MeshPosition + renderer.LevelMesh[2].Vertexdata[1].Position;
	//const glm::vec3 TopRightVertex2 = renderer.LevelMesh[2].MeshPosition + renderer.LevelMesh[2].Vertexdata[2].Position;
	//const glm::vec3 TopLeftVertex2 = renderer.LevelMesh[2].MeshPosition + renderer.LevelMesh[2].Vertexdata[3].Position;

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		sprite[0].SpriteMesh->MeshPosition.x -= 0.01f;
		//if (TopRightVertex.x >= TopLeftVertex2.x &&
		//	TopRightVertex2.x >= TopLeftVertex.x &&
		//	TopRightVertex.y >= BottomRightVertex2.y &&
		//	TopRightVertex2.y >= BottomRightVertex.y)
		//{
		//	if (TopRightVertex2.x > TopLeftVertex.x)
		//	{
		//		renderer.LevelMesh[1].MeshPosition.x += TopRightVertex2.x;
		//	}
		//}
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		sprite[0].SpriteMesh->MeshPosition.x += 0.01f;
		//if (TopRightVertex.x >= TopLeftVertex2.x &&
		//	TopRightVertex2.x >= TopLeftVertex.x &&
		//	TopRightVertex.y >= BottomRightVertex2.y &&
		//	TopRightVertex2.y >= BottomRightVertex.y)
		//{
		//	if (TopRightVertex.x > TopLeftVertex2.x)
		//	{
		//		renderer.LevelMesh[1].MeshPosition.x -= TopRightVertex2.x;
		//	}
		//}
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		sprite[0].SpriteMesh->MeshPosition.y += 0.01f;
		//if (TopRightVertex.x >= TopLeftVertex2.x &&
		//	TopRightVertex2.x >= TopLeftVertex.x &&
		//	TopRightVertex.y >= BottomRightVertex2.y &&
		//	TopRightVertex2.y >= BottomRightVertex.y)
		//{
		//	if (TopRightVertex.y > BottomRightVertex2.y)
		//	{
		//		renderer.LevelMesh[1].MeshPosition.y -= TopRightVertex2.y;
		//	}
		//}
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		sprite[0].SpriteMesh->MeshPosition.y -= 0.01f;
		//if (TopRightVertex.x >= TopLeftVertex2.x &&
		//	TopRightVertex2.x >= TopLeftVertex.x &&
		//	TopRightVertex.y >= BottomRightVertex2.y &&
		//	TopRightVertex2.y >= BottomRightVertex.y)
		//{
		//	if (TopRightVertex2.y > BottomRightVertex.y)
		//	{
		//		renderer.LevelMesh[1].MeshPosition.y += TopRightVertex2.y;
		//	}
		//}
	}

	//renderer.LevelMesh[2]->MeshPosition = glm::vec3(5.0f, 0.0f, 0.0f);


	//if (TopRightVertex.x >= TopLeftVertex2.x)
	//{
	//	renderer.LevelMesh[1].MeshPosition.x -= 0.01f;
	//}

	//if (TopRightVertex2.x >= TopLeftVertex.x)
	//{
	//	renderer.LevelMesh[1].MeshPosition.x += 0.01f;
	//}

	//if (TopRightVertex.y >= BottomRightVertex2.y)
	//{
	//	renderer.LevelMesh[1].MeshPosition.y -= 0.01f;
	//}

	//if (TopRightVertex2.y >= BottomRightVertex.y)
	//{
	//	renderer.LevelMesh[1].MeshPosition.y += 0.01f;
	//}

	//if (TopRightVertex.x >= TopLeftVertex2.x &&
	//	TopRightVertex2.x >= TopLeftVertex.x &&
	//	TopRightVertex.y >= BottomRightVertex2.y &&
	//	TopRightVertex2.y >= BottomRightVertex.y)
	//{
	//	int a = 34;
	//}

}

void VulkanGraphics2D::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		if (CompareVulkanSettings != renderer.Settings)
		{
			CompareVulkanSettings = renderer.Settings;
			renderer.UpdateSwapChain(Window.GetWindowPtr());
		}

		Window.Update();
		//mouse.Update(Window.GetWindowPtr(), renderer.camera, renderer.Settings);
		keyboard.UpdateOrtho(Window.GetWindowPtr(), renderer.OrthoCamera);
		UpdateImGUI();
		Update(renderer.DrawFrame);
		renderer.Draw(Window.GetWindowPtr());
	}
}