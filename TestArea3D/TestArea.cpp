#include "TestArea.h"
#include <FileSystem.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ImGui/imgui_impl_vulkan.h"

#include "Sprite.h"
#include "LevelSprite.h"
#include <TextureRenderedSprite.h>

VulkanGraphics::VulkanGraphics(int Width, int Height, const char* AppName)
{
	Window = VulkanWindow(Width, Height, AppName);
	renderer = RendererManager(Window.GetWindowPtr());
	gameManager = GameManager(renderer);

	CameraList.emplace_back(std::make_shared<PerspectiveCamera>(PerspectiveCamera(glm::vec2(1920, 1080), glm::vec3(2.0f, 7.0, 26.0f))));
	ActiveCamera = CameraList[cameraIndex];

	light = Light(renderer, gameManager.textureManager, RenderBitFlag::RenderOnMainPass | RenderBitFlag::RenderOnTexturePass, glm::vec3(0.0f));
	mesh = Model(renderer, gameManager.textureManager, "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/Demon/Demons.fbx");
	//Scene = Model(renderer, gameManager.textureManager, "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/Sponza/sponza.obj");
	//skybox = SkyBox(renderer, gameManager.textureManager, SparkManTextures);

	//bloomRenderPass = BloomRenderPass(renderer, gameManager.textureManager, renderer.sceneRenderer.BloomTexture);
	//framebuffer3 = FrameBufferMesh(renderer, gameManager.textureManager, renderer.sceneRenderer.ColorTexture, bloomRenderPass.OutputBloomImage());
}

VulkanGraphics::~VulkanGraphics()
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);

	gameManager.textureManager->UnloadAllTextures(*renderer.GetVulkanRendererBase());
	light.Destory(renderer);
	mesh.Destroy(renderer);
	//Scene.Destroy(renderer);
	//skybox.Destory(renderer);
	//bloomRenderPass.Destory(renderer);
	//framebuffer3.Destory(renderer);
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
		ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
		ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);

		ImGui::SliderFloat3("dLight", &light.light.dLight.direction.x, -10.0f, 10.0f);
		ImGui::SliderFloat3("dambient", &light.light.dLight.ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("ddiffuse", &light.light.dLight.diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("dspecular", &light.light.dLight.specular.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pLight", &light.light.pLight[0].position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("pambient", &light.light.pLight[0].ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pdiffuse", &light.light.pLight[0].diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pspecular", &light.light.pLight[0].specular.x, 0.0f, 1.0f);

		ImGui::SliderFloat3("pLight2", &light.light.pLight[1].position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("pambient2", &light.light.pLight[1].ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pdiffuse2", &light.light.pLight[1].diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pspecular2", &light.light.pLight[1].specular.x, 0.0f, 1.0f);

		ImGui::SliderFloat3("pLight3", &light.light.pLight[2].position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("pambient3", &light.light.pLight[2].ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pdiffuse3", &light.light.pLight[2].diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pspecular3", &light.light.pLight[2].specular.x, 0.0f, 1.0f);

		ImGui::SliderFloat3("pLight4", &light.light.pLight[3].position.x, -100.0f, 100.0f);
		ImGui::SliderFloat3("pambient4", &light.light.pLight[3].ambient.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pdiffuse4", &light.light.pLight[3].diffuse.x, 0.0f, 1.0f);
		ImGui::SliderFloat3("pspecular4", &light.light.pLight[3].specular.x, 0.0f, 1.0f);
		ImGui::End();

		gameManager.textureManager->UpdateIMGUIVRAM();
		mesh.UpdateImGUI();
		//Scene.UpdateImGUI();
	}

	ImGui::Render();
}

void VulkanGraphics::Update(uint32_t DrawFrame, std::shared_ptr<Camera> camera)
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	camera->Update();
	//bloomRenderPass.Update(renderer);
	//framebuffer3.Update(renderer);

	light.Update(renderer, camera);

	mesh.Update(renderer, camera, light.light);
	//Scene.Update(renderer, camera, light.light);
	//skybox.Update(renderer, camera);
}

void VulkanGraphics::Draw()
{
	renderer.StartDraw(Window.GetWindowPtr());
	if (renderer.UpdateSwapChainFlag)
	{
		ScreenResizeUpdate();
	}
	Update(renderer.DrawFrame, ActiveCamera);
	renderer.MainRenderPass();
	//renderer.ShadowRenderPass();
	//renderer.SceneRenderPass();
	//bloomRenderPass.Draw(renderer);
	//renderer.FrameBufferRenderPass();
	renderer.EndDraw(Window.GetWindowPtr());
	if (renderer.UpdateSwapChainFlag)
	{
		ScreenResizeUpdate();
	}
}

void VulkanGraphics::ScreenResizeUpdate()
{
	renderer.ScreenResizeUpdate(Window.GetWindowPtr());
	//bloomRenderPass.UpdateSwapChain(renderer, gameManager.textureManager, renderer.sceneRenderer.BloomTexture);
	//framebuffer3.ScreenResizeUpdate(renderer, gameManager.textureManager, renderer.sceneRenderer.ColorTexture, bloomRenderPass.OutputBloomImage());

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