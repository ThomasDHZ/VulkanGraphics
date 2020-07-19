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
#include "Mesh2.h"

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
	std::vector<VkCommandBuffer> RunCommandBuffers = {};

	GUIDebugger guiDebugger;

	void UpdateSwapChain(GLFWwindow* window);
	uint32_t Draw(GLFWwindow* window, std::vector<Mesh2>& MeshList);
	void DestoryVulkan();

	bool UpdateCommandBuffers = true;

public:

	Renderer();
	Renderer(GLFWwindow* window);
	~Renderer();

	VulkanRendererSettings Settings;

};
