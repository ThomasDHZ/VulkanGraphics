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

class Renderer : public VulkanRenderer
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class VulkanResources;
private:

	size_t currentFrame = 0;
	bool framebufferResized = false;

	GUIDebugger guiDebugger;
	ForwardRenderer forwardRenderer;
	TextureRenderer textureRenderer;
	FramebufferRenderer frameBufferRenderer;

	void InitializeGUIDebugger(GLFWwindow* window);
	void DrawToTextureRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void MainRenderPass(SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void FrameBufferRenderPass(FrameBufferMesh framebuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void UpdateSwapChain(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);

protected:

	uint32_t Draw(GLFWwindow* window, FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh2>& MeshList);
	void DestoryVulkan();

public:

	Renderer();
	Renderer(GLFWwindow* window);
	~Renderer();

	VulkanRenderer* GetVulkanRendererBase() { return static_cast<VulkanRenderer*>(this); }
};
