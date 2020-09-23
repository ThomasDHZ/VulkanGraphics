#pragma once
#include "VulkanEngine.h"
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

class RendererManager : public VulkanEngine
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class Level2D;
private:

	size_t currentFrame = 0;
	bool framebufferResized = false;
	bool UpdateSwapChainFlag = false;

	GUIDebugger guiDebugger;

	void InitializeGUIDebugger(GLFWwindow* window);
	//void CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh>& MeshList);
	void UpdateSwapChain(GLFWwindow* window);

	void DrawToTextureRenderPass();
	void MainRenderPass();
	void SceneRenderPass();
	void FrameBufferRenderPass();
	void EffectRenderPass();
	void EffectRenderPass2();
	void ShadowRenderPass();
protected:

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
	FrameBufferTextureRenderer EffectRenderer2;

	void RemoveDrawMessage(std::shared_ptr<RendererDrawMessage> mesh);
	VulkanEngine* GetVulkanRendererBase() { return static_cast<VulkanEngine*>(this); }
};
