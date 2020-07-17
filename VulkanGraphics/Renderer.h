#pragma once
#include "RendererBase.h"
#include "VulkanDebugger.h"
#include "VulkanSwapChain.h"
#include "ForwardRenderingPipeline.h"
#include "InputAttachment.h"
#include "SkyBoxPipeline.h"
#include "WireFramePipeline.h"
#include "FrameBufferMesh.h"
#include "FrameBufferRenderingPipeline.h"
#include "DebugLightRenderingPipeline.h"
#include "CollisionDebugPipeline.h"
#include "GUIDebugger.h"
#include "ForwardRenderer.h"
#include "TextureRenderer.h"
#include "TextureSkyboxRenderer.h"
class Renderer : public RendererBase
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class VulkanResources;
private:

	bool framebufferResized = false;


	ForwardRenderer forwardRenderer;
	TextureRenderer textureRenderer;
	TextureSkyboxRenderer textureSkyboxRenderer;
	//ForwardRenderingPipeline GraphicsPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline MeshviewPipeline;
	SkyBoxPipeline SkyboxPipeline;

	void InitializeGUIDebugger(GLFWwindow* window);

protected:

	//Pretty much to section off for anything that needs to be used in VUlkanGraphics.
	size_t currentFrame = 0;
	std::vector<VkCommandBuffer> RunCommandBuffers = {};

	GUIDebugger guiDebugger;

	void UpdateSwapChain(GLFWwindow* window);
	uint32_t DrawStart(GLFWwindow* window);
	void DrawEnd(GLFWwindow* window, uint32_t drawFrame);
	void DestoryVulkan();

	bool UpdateCommandBuffers = true;

public:

	Renderer();
	Renderer(GLFWwindow* window);
	~Renderer();

	VulkanRendererSettings Settings;
	uint32_t DrawFrame = 0;
};
