#include "VulkanGraphics2D.h"
#include <FileSystem.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include "ImGui/imgui_impl_vulkan.h"
#include "Sprite.h"
#include "Coin.h"

VulkanGraphics2D::VulkanGraphics2D(int Width, int Height, const char* AppName)
{

	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	gameManager = GameManager(renderer);

	MeshTextures SparkManTextures = {};
	SparkManTextures.DiffuseMap = "texture/SparkMan_diffuseOriginal.bmp";
	SparkManTextures.SpecularMap = "texture/SparkManSpecular.bmp";
	SparkManTextures.NormalMap = "texture/SparkMan_normal.bmp";
	SparkManTextures.AlphaMap = "texture/SparkManAlpha.bmp";


	//spriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, gameManager.textureManager, MegaManVertices, MegaManIndices, MegaManTextures, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));
	light = Light(renderer, renderer.forwardRenderer.DebugLightPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderOnTexturePass, glm::vec3(0.0f));
	SpriteList.emplace_back(std::make_shared<MegaMan>(MegaMan(renderer, gameManager.textureManager, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, glm::vec2(0.0f, 0.0f))));
	SpriteList.emplace_back(std::make_shared<Coin>(Coin(renderer, gameManager.textureManager, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, glm::vec2(5.0f, 6.0f))));
	SpriteList.emplace_back(std::make_shared<Coin>(Coin(renderer, gameManager.textureManager, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, glm::vec2(3.0f, 3.0f))));
	
	level = LevelSprite(renderer, gameManager.textureManager, SparkManTextures, renderer.forwardRenderer.renderer2DPipeline.ShaderPipelineDescriptorLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass);
}

VulkanGraphics2D::~VulkanGraphics2D()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	gameManager.textureManager->UnloadAllTextures(*renderer.GetVulkanRendererBase());
	light.Destory(renderer);
	for (auto sprite : SpriteList)
	{
		sprite->Destory(renderer);
	}
	level.Destory(renderer);

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

			for (int x = 0; x < renderer.ObjectMesh.size(); x++)
			{
				bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Mesh%d", x);
				ImGui::NextColumn();
				ImGui::Text("Node contents");
				ImGui::NextColumn();
				if (open1)
				{
					if (const auto mesh = dynamic_cast<Mesh2D*>(renderer.ObjectMesh[x].get()))
					{
						ImGui::SliderFloat2("shininess", &mesh->properites.UVOffset.x, 0.0, 1.0f);
						ImGui::SliderFloat("shininess", &mesh->properites.material.shininess, 0.0, 255.0f);
						ImGui::SliderFloat("reflectivness", &mesh->properites.material.reflectivness, 0.0f, 1.0f);
						ImGui::SliderFloat("heightScale", &mesh->properites.heightScale, -1.0, 1.0f);
						ImGui::SliderFloat("Layers", &mesh->properites.minLayers, 0.0, 50.0f);
						ImGui::SliderFloat("maxLayers", &mesh->properites.maxLayers, 0.0, 5000.0f);
						ImGui::SliderInt("Diffuse", &mesh->properites.UseDiffuseMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Specular", &mesh->properites.UseSpecularMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Normal", &mesh->properites.UseNormalMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Depth", &mesh->properites.UseDepthMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Alpha", &mesh->properites.UseAlphaMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Emission", &mesh->properites.UseEmissionMapBit, 0.0, 1.0f);
						ImGui::SliderInt("Reflection", &mesh->properites.UseReflectionMapBit, 0.0, 1.0f);
						ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->DiffuseMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
						ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->SpecularMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
						ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->NormalMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
						ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->DepthMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
						ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->ReflectionMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
						ImGui::NextColumn();
					}
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

void VulkanGraphics2D::Update(uint32_t DrawFrame)
{
	bool Collide = false;
	glm::vec3 MoveDirection = glm::vec3(0.0f);

	light.Update(renderer, renderer.OrthoCamera);
	level.Update(renderer, renderer.OrthoCamera, light.light);
	for (auto& sprite : SpriteList)
	{
		sprite->Update(renderer, renderer.OrthoCamera, light.light);
		//sprite.get()->Collision(renderer, SpriteList);
	}
	//for (auto& sprite : SpriteList)
	//{
	//	if (sprite.get() != SpriteList[0].get())
	//	{
	//		if (!SpriteList[0]->collider.CollidesWith(sprite->collider, MoveDirection))
	//		{
	//			if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_LEFT) == GLFW_PRESS)
	//			{
	//				MoveDirection = glm::vec3(-0.01f, 0.0f, 0.0f);
	//			}
	//			else if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	//			{
	//				MoveDirection = glm::vec3(0.01f, 0.0f, 0.0f);
	//			}
	//			if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_DOWN) == GLFW_PRESS)
	//			{
	//				MoveDirection = glm::vec3(0.0f, -0.01f, 0.0f);
	//			}
	//			else if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_UP) == GLFW_PRESS)
	//			{
	//				MoveDirection = glm::vec3(0.0f, 0.01f, 0.0f);
	//			}
	//		}
	//		else
	//		{
	//			MoveDirection = glm::vec3(0.0f);
	//			break;
	//		}
	//	}
	//}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (!SpriteList[0]->collider.CollidesWith(SpriteList[1]->collider, glm::vec3(-0.01f, 0.0f, 0.0f)) &&
			!SpriteList[0]->collider.CollidesWith(SpriteList[2]->collider, glm::vec3(-0.01f, 0.0f, 0.0f)))
		{
			SpriteList[0]->SpriteMesh->MeshPosition += glm::vec3(-0.01f, 0.0f, 0.0f);
		}
	}
	else if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (!SpriteList[0]->collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.01f, 0.0f, 0.0f)) &&
			!SpriteList[0]->collider.CollidesWith(SpriteList[2]->collider, glm::vec3(0.01f, 0.0f, 0.0f)))
		{
			SpriteList[0]->SpriteMesh->MeshPosition += glm::vec3(0.01f, 0.0f, 0.0f);
		}
	}
	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (!SpriteList[0]->collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.0f, -0.01f, 0.0f)) &&
			!SpriteList[0]->collider.CollidesWith(SpriteList[2]->collider, glm::vec3(0.0f, -0.01f, 0.0f)))
		{
			SpriteList[0]->SpriteMesh->MeshPosition += glm::vec3(0.0f, -0.01f, 0.0f);
		}
	}
	else if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (!SpriteList[0]->collider.CollidesWith(SpriteList[1]->collider, glm::vec3(0.0f, 0.01f, 0.0f)) &&
			!SpriteList[0]->collider.CollidesWith(SpriteList[2]->collider, glm::vec3(0.0f, 0.01f, 0.0f)))
		{
			SpriteList[0]->SpriteMesh->MeshPosition += glm::vec3(0.0f, 0.01f, 0.0f);
		}
	}

	SpriteList[0]->SpriteMesh->MeshPosition += MoveDirection;
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