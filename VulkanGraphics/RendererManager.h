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
#include "Mesh.h"
#include "SkyBoxMesh.h"
#include "FramebufferRenderer.h"
#include "ShadowRenderer.h"
#include <chrono>
#include "DebugLightMesh.h"

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
	//void CMDBuffer(FrameBufferMesh frameBuffer, SkyBoxMesh skybox, std::vector<Mesh>& MeshList);
	void UpdateSwapChain(GLFWwindow* window);

	void DrawToTextureRenderPass(std::vector<Mesh>& MeshList, SkyBoxMesh skybox);
	void MainRenderPass(std::vector<Mesh>& MeshList, SkyBoxMesh skybox, DebugLightMesh debugLight);
	void FrameBufferRenderPass();
	void ShadowRenderPass(std::vector<Mesh>& MeshList);
protected:

	uint32_t Draw(GLFWwindow* window, std::vector<Mesh>& MeshList, SkyBoxMesh skybox, DebugLightMesh debugLight);
	void DestoryVulkan();

public:

	RendererManager();
	RendererManager(GLFWwindow* window);
	~RendererManager();

	FrameBufferMesh frameBuffer;

	void Update(uint32_t DrawFrame, Camera camera, std::vector<Mesh>& MeshList, SkyBoxMesh skybox, DebugLightMesh debugLight);
	VulkanRenderer* GetVulkanRendererBase() { return static_cast<VulkanRenderer*>(this); }
};
