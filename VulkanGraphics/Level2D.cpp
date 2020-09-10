#include "Level2D.h"
#include "VulkanGraphics.h"
#include "MegaMan.h"
#include "Coin.h"
#include "WaterSurface2D.h"
#include "Water2D.h"

Level2D::Level2D()
{
}

Level2D::Level2D(RendererManager& renderer, GameManager& gameManager)
{
	MeshTextures SparkManTextures = {};
	SparkManTextures.DiffuseMap = "texture/SparkMan_diffuseOriginal.bmp";
	SparkManTextures.SpecularMap = "texture/SparkManSpecular.bmp";
	SparkManTextures.NormalMap = "texture/SparkMan_normal.bmp";
	SparkManTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	OrthoCamera = OrthographicCamera(9.0f, 4.5f);
	OrthoCamera2 = OrthographicCamera(9.0f, 4.5f);

	OrthoCamera.SetPosition(2.5f, 8.5f);
	OrthoCamera2.SetPosition(20.5f, 8.5f);

	light = Light(renderer, renderer.forwardRenderer.DebugLightPipeline->ShaderPipelineDescriptorLayout, RenderBitFlag::RenderOnMainPass | RenderBitFlag::RenderOnTexturePass, glm::vec3(0.0f));
	SpriteList.emplace_back(std::make_shared<MegaMan>(MegaMan(renderer, gameManager.textureManager, glm::vec2(1.0f, 10.0f))));
	SpriteList.emplace_back(std::make_shared<Coin>(Coin(renderer, gameManager.textureManager, glm::vec2(5.0f, 8.0f))));
	SpriteList.emplace_back(std::make_shared<Coin>(Coin(renderer, gameManager.textureManager, glm::vec2(3.0f, 8.0f))));
	SpriteList.emplace_back(std::make_shared<WaterSurface2D>(WaterSurface2D(renderer, gameManager.textureManager, glm::vec2(-10.0f, 3.0f), glm::vec2(10.0f, 10.0f), renderer.textureRenderer.ColorTexture)));
	//SpriteList.emplace_back(std::make_shared<Water2D>(Water2D(renderer, gameManager.textureManager, glm::vec2(-6.5f, 4.0f), glm::vec2(9.0f * 2, 4.5f * 2), renderer.textureRenderer.ColorTexture)));
	level = LevelSprite(renderer, gameManager.textureManager, SparkManTextures);
}

Level2D::~Level2D()
{
}

void Level2D::LevelDebug(RendererManager& renderer)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{

		ImGui::Begin("Settings");
		auto a = static_cast<WaterSurface2D*>(SpriteList[3].get());
		ImGui::SliderFloat3("water color", &a->waveprop.WaterColor.x, 0.0f, 1.0f);
		ImGui::SliderFloat2("Distortion_scale", &a->waveprop.Distortion_scale.x, 0.0f, 1.0f);
		ImGui::SliderFloat("intensity", &a->waveprop.intensity, 0.0f, 1.0f);
		ImGui::SliderFloat("speed", &a->waveprop.speed, 0.0f, 1.0f);
		ImGui::SliderFloat("waveAmp", &a->waveprop.waveAmp, 0.0f, 1.0f);
		ImGui::SliderFloat("waveSpeed", &a->waveprop.waveSpeed, 0.0f, 1.0f);
		ImGui::SliderFloat("wavePeriod", &a->waveprop.wavePeriod, 0.0f, 60.0f);

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
		ImGui::Image(renderer.textureRenderer.ColorTexture->ImGuiDescriptorSet, ImVec2(400.0f, 255.0f));
		ImGui::End();

		ImGui::Begin("MeshSettings");
		if (ImGui::TreeNode("MeshList"))
		{
			ImGui::Columns(2, "tree", true);

			/*for (int x = 0; x < renderer.ObjectMesh.size(); x++)
			{
				bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Mesh%d", x);
				ImGui::NextColumn();
				ImGui::Text("Node contents");
				ImGui::NextColumn();
				if (open1)
				{

					ImGui::NextColumn();
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
					}
					ImGui::NextColumn();
					ImGui::TreePop();
				}
			}*/
		}
		ImGui::End();
		//gameManager.textureManager->UpdateIMGUIVRAM();
	}

	ImGui::Render();
}

void Level2D::LevelLoop(RendererManager& renderer, GLFWwindow* Window)
{
	//mouse.Update(Window, OrthoCamera, renderer.Settings);
	keyboard.UpdateOrtho(Window, OrthoCamera);
	Draw(renderer, Window);
	LevelDebug(renderer);
}

void Level2D::Update(RendererManager& renderer, OrthographicCamera& camera)
{
	light.Update(renderer, camera);
	level.Update(renderer, camera, light.light);
	for (int x = SpriteList.size() - 1; x > 0; x--)
	{
		if (SpriteList[x]->SpriteMesh->GetMeshDeletedFlag())
		{
			SpriteList.erase(SpriteList.begin() + x);
		}
	}
	for (auto& sprite : SpriteList)
	{
		if (auto MM = dynamic_cast<MegaMan*>(sprite.get()))
		{
			//MM->Update(Window.GetWindowPtr(), renderer, camera, light.light, SpriteList, level.LevelColliderList, gameManager.textureManager);
		}

		sprite->ApplyGravity(level.TileColliderList);
		sprite->Update(renderer, camera, light.light);
		sprite->Collision(renderer, SpriteList);
	}
}

void Level2D::Draw(RendererManager& renderer, GLFWwindow* Window)
{

	renderer.StartDraw(Window);
	//ShadowRenderPass();
	if (renderer.currentFrame == 1)
	{
		Update(renderer, OrthoCamera);
		renderer.DrawToTextureRenderPass();
	}
	else
	{
		Update(renderer, OrthoCamera);
		renderer.MainRenderPass();
	}
	//FrameBufferRenderPass();
	//renderer.Draw(Window.GetWindowPtr());
	renderer.EndDraw(Window);
}

void Level2D::Destroy(RendererManager& renderer)
{
	light.Destory(renderer);
	for (auto sprite : SpriteList)
	{
		sprite->Destory(renderer);
	}
	level.Destory(renderer);
}
