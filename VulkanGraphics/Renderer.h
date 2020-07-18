#pragma once
#include "VulkanRenderer.h"
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

class Renderer : public VulkanRenderer
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class VulkanResources;
private:

	bool framebufferResized = false;


	ForwardRenderer forwardRenderer;
	TextureRenderer textureRenderer;

	//ForwardRenderingPipeline GraphicsPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline MeshviewPipeline;
	SkyBoxPipeline SkyboxPipeline;

	void InitializeGUIDebugger(GLFWwindow* window);

protected:

	//Pretty much to section off for anything that needs to be used in VUlkanGraphics.
	size_t currentFrame = 0;

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
};
