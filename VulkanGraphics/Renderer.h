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

class Renderer : public RendererBase
{
	friend class VulkanGraphics;
	friend class VulkanGraphics2D;
	friend class VulkanResources;
private:

	bool framebufferResized = false;


	VkRenderPass RenderPass = VK_NULL_HANDLE;

	InputAttachment ColorAttachment;
	InputAttachment DepthAttachment;

	VulkanSwapChain SwapChain;

	ForwardRenderingPipeline GraphicsPipeline;
	FrameBufferRenderingPipeline FrameBufferPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline MeshviewPipeline;
	SkyBoxPipeline SkyboxPipeline;


	FrameBufferMesh framebuffer;

	void InitializeRenderPass();
	void InitializeFramebuffers();
	void InitializeCommandBuffers();
	void InitializeSyncObjects();

protected:

	//Pretty much to section off for anything that needs to be used in VUlkanGraphics.
	size_t currentFrame = 0;
	std::vector<VkCommandBuffer> RunCommandBuffers = {};
	std::vector<VkFramebuffer> SwapChainFramebuffers;

	void UpdateSwapChain(GLFWwindow* window);
	void StartFrame(GLFWwindow* window);
	void EndFrame(GLFWwindow* window);
	void DestoryVulkan();

	bool UpdateCommandBuffers = true;

public:

	Renderer();
	Renderer(GLFWwindow* window);
	~Renderer();

	VulkanRendererSettings Settings;
	uint32_t DrawFrame = 0;
};
