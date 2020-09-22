#pragma once
#include "VulkanRenderer.h"
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include "SkyBoxPipeline.h"
#include "WireFramePipeline.h"
#include "GUIDebugger.h"
#include "ForwardRenderer.h"
#include "TextureRenderer.h"
#include "FramebufferRenderer.h"
#include "ShadowRenderer.h"
#include <chrono>
#include "SceneRenderer.h"
#include "FrameBufferTextureRenderer.h"

class RendererManager : public VulkanRenderer
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class Level2D;
private:

	size_t currentFrame = 0;
	bool framebufferResized = false;

	GUIDebugger guiDebugger;
	//Camera lightCamera;
	//Camera camera;

	void InitializeGUIDebugger(GLFWwindow* window);
	//void CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh>& MeshList);
	void UpdateSwapChain(GLFWwindow* window);

	//void DrawToBloomTextureRenderPass();
	void DrawToTextureRenderPass();
	void MainRenderPass();
	void SceneRenderPass();
	void FrameBufferRenderPass();
	void EffectRenderPass();
	void ShadowRenderPass();
protected:

	uint32_t Draw(GLFWwindow* window);
	void StartDraw(GLFWwindow* window);
	void EndDraw(GLFWwindow* window);
	void DestoryVulkan();

public:

	RendererManager();
	RendererManager(GLFWwindow* window);
	~RendererManager();

	std::vector<std::shared_ptr<RendererDrawMessage>> DrawMessageList;
	ForwardRenderer forwardRenderer;
	SceneRenderer sceneRenderer;
	TextureRenderer textureRenderer;
	ShadowRenderer shadowRenderer;
	FramebufferRenderer frameBufferRenderer;
	FrameBufferTextureRenderer EffectRenderer;

	void RemoveDrawMessage(std::shared_ptr<RendererDrawMessage> mesh);
	VulkanRenderer* GetVulkanRendererBase() { return static_cast<VulkanRenderer*>(this); }
};
