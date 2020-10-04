#include "VulkanGraphics.h"
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ImGui/imgui_impl_vulkan.h"

#include "Sprite.h"
#include "EnergyTank.h"
#include "LargeEnergy.h"
#include "WaterSurface2D.h"
#include "Water2D.h"
#include "MegaMan.h"
#include "Enemy2D.h"
#include "LevelSprite.h"

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	gameManager = GameManager(renderer);

	MeshTextures SparkManTextures = {};
	SparkManTextures.DiffuseMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkMan_diffuseOriginal.bmp";
	SparkManTextures.SpecularMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkManSpecular.bmp";
	SparkManTextures.NormalMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkMan_normal.bmp";
	SparkManTextures.AlphaMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkManAlpha.bmp";
	SparkManTextures.DepthMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkManAlpha.bmp";
	SparkManTextures.ReflectionMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/SparkManAlpha.bmp";
	SparkManTextures.CubeMap[0] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/left.jpg";
	SparkManTextures.CubeMap[1] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/right.jpg";
	SparkManTextures.CubeMap[2] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/top.jpg";
	SparkManTextures.CubeMap[3] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/bottom.jpg";
	SparkManTextures.CubeMap[4] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/back.jpg";
	SparkManTextures.CubeMap[5] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/front.jpg";

	CameraList.emplace_back(std::make_shared<OrthographicCamera>(OrthographicCamera(glm::vec2(1920, 1080), glm::vec2(8.0f, 9.0f), 4.0f)));
	CameraList.emplace_back(std::make_shared<OrthographicCamera>(OrthographicCamera(glm::vec2(1920, 1080), glm::vec2(8.0f, 12.0f), 4.0f)));
	CameraList.emplace_back(std::make_shared<OrthographicCamera>(OrthographicCamera(glm::vec2(1920, 1080), glm::vec2(10.0f, 12.0f), 4.0f)));
	ActiveCamera = CameraList[cameraIndex];

	MeshTextures meshTextures = {};
	meshTextures.DiffuseMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/container2.png";
	meshTextures.SpecularMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png";
	meshTextures.NormalMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/brick_normal.bmp";
	meshTextures.DepthMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/brick_height.bmp";
	meshTextures.ReflectionMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/container2_specular.png";
	meshTextures.EmissionMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/matrix.jpg";
	meshTextures.CubeMap[0] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/left.jpg";
	meshTextures.CubeMap[1] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/right.jpg";
	meshTextures.CubeMap[2] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/top.jpg";
	meshTextures.CubeMap[3] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/bottom.jpg";
	meshTextures.CubeMap[4] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/back.jpg";
	meshTextures.CubeMap[5] = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/skybox/front.jpg";

	light = Light(renderer, gameManager.textureManager, RenderBitFlag::RenderOnMainPass | RenderBitFlag::RenderOnTexturePass, glm::vec3(0.0f));
	SpriteList.emplace_back(std::make_shared<MegaMan>(MegaMan(renderer, gameManager.textureManager, glm::vec2(1.0f, 10.0f))));
	SpriteList.emplace_back(std::make_shared<EnergyTank>(EnergyTank(renderer, gameManager.textureManager, glm::vec2(5.0f, 8.0f))));
	SpriteList.emplace_back(std::make_shared<EnergyTank>(EnergyTank(renderer, gameManager.textureManager, glm::vec2(3.0f, 8.0f))));
	SpriteList.emplace_back(std::make_shared<LargeEnergy>(LargeEnergy(renderer, gameManager.textureManager, glm::vec2(9.0f, 8.0f))));
	SpriteList.emplace_back(std::make_shared<Enemy2D>(Enemy2D(renderer, gameManager.textureManager, glm::vec2(8.0f, 10.0f))));
	/*SpriteList.emplace_back(std::make_shared<WaterSurface2D>(WaterSurface2D(renderer, gameManager.textureManager, glm::vec2(-10.0f, 3.0f), glm::vec2(10.0f, 10.0f), renderer.sceneRenderer.BloomTexture)));*/
	//SpriteList.emplace_back(std::make_shared<Water2D>(Water2D(renderer, gameManager.textureManager, glm::vec2(-6.5f, 4.0f), glm::vec2(18.0f, 4.5f * 2), CameraList[1])));
	//SpriteList.emplace_back(std::make_shared<Water2D>(Water2D(renderer, gameManager.textureManager, glm::vec2(-6.5f, 4.0f), glm::vec2(18.0f, 4.5f * 2), CameraList[2])));
	SpriteList.emplace_back(std::make_shared<LevelSprite>(LevelSprite(renderer, gameManager.textureManager, SparkManTextures)));

	bloomRenderPass = BloomRenderPass(renderer, gameManager.textureManager, renderer.sceneRenderer.BloomTexture);
	framebuffer3 = FrameBufferMesh(renderer, gameManager.textureManager, renderer.sceneRenderer.ColorTexture, bloomRenderPass.OutputBloomImage());
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	gameManager.textureManager->UnloadAllTextures(*renderer.GetVulkanRendererBase());
	light.Destory(renderer);
	for (auto sprite : SpriteList)
	{
		sprite->Destory(renderer);
	}
	bloomRenderPass.Destory(renderer);
	framebuffer3.Destory(renderer);
	renderer.DestoryVulkan();
	Window.CleanUp();
}

void VulkanGraphics::UpdateImGUI()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		alignas(16) glm::vec3 WaterColor = glm::vec3(0.7f, 0.0, 0.4f);
		alignas(8) glm::vec2 Distortion_scale = glm::vec2(.389f, .053f);
		alignas(4) float intensity = 00.3f;
		alignas(4)float speed = 0.085f;
		alignas(4) float waveAmp = 0.238f;
		alignas(4) float waveSpeed = 4.0f;
		alignas(4) float wavePeriod = 2.423f;


		ImGui::Begin("Settings");
		//auto a = static_cast<WaterSurface2D*>(SpriteList[3].get());
		//ImGui::SliderFloat3("water color", &a->waveprop.WaterColor.x, 0.0f, 1.0f);
		//ImGui::SliderFloat2("Distortion_scale", &a->waveprop.Distortion_scale.x, 0.0f, 1.0f);
		//ImGui::SliderFloat("intensity", &a->waveprop.intensity, 0.0f, 1.0f);
		//ImGui::SliderFloat("speed", &a->waveprop.speed, 0.0f, 1.0f);
		//ImGui::SliderFloat("waveAmp", &a->waveprop.waveAmp, 0.0f, 1.0f);
		//ImGui::SliderFloat("waveSpeed", &a->waveprop.waveSpeed, 0.0f, 1.0f);
		//ImGui::SliderFloat("wavePeriod", &a->waveprop.wavePeriod, 0.0f, 60.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::SliderFloat("Gamma", &framebuffer.settings.Gamma, 0.0f, 10.0f);
		//ImGui::SliderFloat("HDR Value", &framebuffer.settings.HDRValue, 0.0f, 10.0f);
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
		for (auto& sprite : SpriteList)
		{
			if (auto spriteTextureRenderer = dynamic_cast<TextureRenderedSprite*>(sprite.get()))
			{
				ImGui::Image(spriteTextureRenderer->GetIMGuiImage(), ImVec2(400.0f, 255.0f));
			}
		}
		ImGui::Image(bloomRenderPass.GetIMGuiImagePass2(), ImVec2(400.0f, 255.0f));
		ImGui::Image(renderer.shadowRenderer.DepthTexture->ImGuiDescriptorSet, ImVec2(400.0f, 255.0f));
		ImGui::End();

		ImGui::Begin("MeshSettings");
		ImGui::SliderInt("Camera", &cameraIndex, 0, CameraList.size() - 1);
		ActiveCamera = CameraList[cameraIndex];

		if (ImGui::TreeNode("MeshList"))
		{
			//ImGui::Columns(2, "tree", true);

			//for (int x = 0; x < renderer.ObjectMesh.size(); x++)
			//{
			//	bool open1 = ImGui::TreeNode((void*)(intptr_t)x, "Mesh%d", x);
			//	ImGui::NextColumn();
			//	ImGui::Text("Node contents");
			//	ImGui::NextColumn();
			//	if (open1)
			//	{

			//		ImGui::NextColumn();
			//		if (const auto mesh = dynamic_cast<Mesh2D*>(renderer.ObjectMesh[x].get()))
			//		{
			//			ImGui::SliderFloat2("shininess", &mesh->properites.UVOffset.x, 0.0, 1.0f);
			//			ImGui::SliderFloat("shininess", &mesh->properites.material.shininess, 0.0, 255.0f);
			//			ImGui::SliderFloat("reflectivness", &mesh->properites.material.reflectivness, 0.0f, 1.0f);
			//			ImGui::SliderFloat("heightScale", &mesh->properites.heightScale, -1.0, 1.0f);
			//			ImGui::SliderFloat("Layers", &mesh->properites.minLayers, 0.0, 50.0f);
			//			ImGui::SliderFloat("maxLayers", &mesh->properites.maxLayers, 0.0, 5000.0f);
			//			ImGui::SliderInt("Diffuse", &mesh->properites.UseDiffuseMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Specular", &mesh->properites.UseSpecularMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Normal", &mesh->properites.UseNormalMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Depth", &mesh->properites.UseDepthMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Alpha", &mesh->properites.UseAlphaMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Emission", &mesh->properites.UseEmissionMapBit, 0.0, 1.0f);
			//			ImGui::SliderInt("Reflection", &mesh->properites.UseReflectionMapBit, 0.0, 1.0f);
			//			ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->DiffuseMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
			//			ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->SpecularMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
			//			ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->NormalMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
			//			ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->DepthMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
			//			ImGui::Image(gameManager.textureManager->GetTextureByID(renderer.ObjectMesh[x]->ReflectionMapID).ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
			//		}
			//		ImGui::NextColumn();
			//		ImGui::TreePop();
			//	}
			//}
		}
		ImGui::End();
		gameManager.textureManager->UpdateIMGUIVRAM();
	}

	ImGui::Render();
}

void VulkanGraphics::Update(uint32_t DrawFrame, std::shared_ptr<Camera> camera)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	camera->Update();
	bloomRenderPass.Update(renderer);
	framebuffer3.Update(renderer);

	light.Update(renderer, camera);
	for (int x = SpriteList.size() - 1; x > 0; x--)
	{
		if (SpriteList[x]->DestoryObjectFlag)
		{
			SpriteList[x]->Destory(renderer);
			SpriteList.erase(SpriteList.begin() + x);
		}
	}
	for (auto& sprite : SpriteList)
	{
		if (auto MM = dynamic_cast<MegaMan*>(sprite.get()))
		{
			MM->Update(Window.GetWindowPtr(), renderer, camera, deltaTime, light.light, SpriteList, gameManager.textureManager);
		}
		else
		{
			sprite->Update(renderer, deltaTime, camera, light.light);
		}
		if (auto MM = dynamic_cast<Sprite*>(sprite.get()))
		{
			MM->ApplyGravity(SpriteList, deltaTime);
		}
		sprite->Collision(renderer, SpriteList);
	}

	if (glfwGetKey(Window.GetWindowPtr(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		//CurrentAni = ShootAni;
		auto shot = std::make_shared<MMShot>(MMShot(renderer, gameManager.textureManager, glm::vec2(SpriteList[0]->GetPosition2D().x + 1.0f, SpriteList[0]->GetPosition2D().y + 0.5f)));
		SpriteList.emplace_back(shot);
	}
}

void VulkanGraphics::Draw()
{
	{
		for (auto& sprite : SpriteList)
		{
			if (auto spriteTextureRenderer = dynamic_cast<TextureRenderedSprite*>(sprite.get()))
			{
				renderer.StartDraw(Window.GetWindowPtr());
				if (renderer.UpdateSwapChainFlag)
				{
					ScreenResizeUpdate();
				}

				Update(renderer.DrawFrame, static_cast<Water2D*>(sprite.get())->WaterCamera);
				spriteTextureRenderer->RendererUpdate(renderer);

				renderer.EndDraw(Window.GetWindowPtr());
				if (renderer.UpdateSwapChainFlag)
				{
					ScreenResizeUpdate();
				}
			}
		}
	}
	{
		renderer.StartDraw(Window.GetWindowPtr());
		if (renderer.UpdateSwapChainFlag)
		{
			ScreenResizeUpdate();
		}
		Update(renderer.DrawFrame, ActiveCamera);
		renderer.MainRenderPass();
		renderer.ShadowRenderPass();
		renderer.SceneRenderPass();
		bloomRenderPass.Draw(renderer);
		renderer.FrameBufferRenderPass();
		renderer.EndDraw(Window.GetWindowPtr());
		if (renderer.UpdateSwapChainFlag)
		{
			ScreenResizeUpdate();
		}
	}
}

void VulkanGraphics::ScreenResizeUpdate()
{

	renderer.ScreenResizeUpdate(Window.GetWindowPtr());
	bloomRenderPass.UpdateSwapChain(renderer, gameManager.textureManager, renderer.sceneRenderer.BloomTexture);
	framebuffer3.ScreenResizeUpdate(renderer, gameManager.textureManager, renderer.sceneRenderer.ColorTexture, bloomRenderPass.OutputBloomImage());

	for (auto& sprite : SpriteList)
	{
		if (auto spriteTextureRenderer = dynamic_cast<TextureRenderedSprite*>(sprite.get()))
		{
			spriteTextureRenderer->UpdateSwapChain(renderer);
		}
	}

	int NewWidth = 0;
	int NewHeight = 0;
	glfwGetFramebufferSize(Window.GetWindowPtr(), &NewWidth, &NewHeight);
	for (auto& camera : CameraList)
	{
		camera->UpdateScreenSize(NewWidth, NewHeight);
	}
}

void VulkanGraphics::MainLoop()
{
	while (!glfwWindowShouldClose(Window.GetWindowPtr()))
	{
		Window.Update();
		mouse.Update(Window.GetWindowPtr(), ActiveCamera);
		keyboard.UpdateOrtho(Window.GetWindowPtr(), ActiveCamera);
		UpdateImGUI();
		Draw();
	}
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