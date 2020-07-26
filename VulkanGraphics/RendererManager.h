#pragma once
#include "VulkanRenderer.h"
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include "SkyBoxPipeline.h"
#include "WireFramePipeline.h"
#include "FrameBufferMesh.h"
#include "FrameBufferRenderingPipeline.h"
#include "DebugLightRenderingPipeline.h"
#include "CollisionDebugPipeline.h"
#include "GUIDebugger.h"
#include "ForwardRenderer.h"
#include "TextureRenderer.h"
#include "Mesh2.h"
#include "SkyBoxMesh.h"
#include "FramebufferRenderer.h"
#include "ShadowRenderer.h"

class RendererManager : public VulkanRenderer
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;

private:

	size_t currentFrame = 0;
	bool framebufferResized = false;

	GUIDebugger guiDebugger;
	ForwardRenderer forwardRenderer;
	TextureRenderer textureRenderer;
	FramebufferRenderer frameBufferRenderer;
	ShadowRenderer shadowRenderer;

	void InitializeGUIDebugger(GLFWwindow* window);
	void CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void UpdateSwapChain(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);

	void DrawToTextureRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void MainRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void FrameBufferRenderPass(FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void ShadowRenderPass(FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
protected:

	uint32_t Draw(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void DestoryVulkan();

public:

	RendererManager();
	RendererManager(GLFWwindow* window);
	~RendererManager();

	VulkanRenderer* GetVulkanRendererBase() { return static_cast<VulkanRenderer*>(this); }
};
