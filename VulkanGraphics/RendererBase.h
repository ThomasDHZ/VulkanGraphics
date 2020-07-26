#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "VulkanRenderer.h"
#include "Mesh2.h"
#include "SkyBoxMesh.h"
#include "SkyBoxPipeline.h"
#include "DebugLightRenderingPipeline.h"
#include "FrameBufferRenderingPipeline.h"
#include "CollisionDebugPipeline.h"
#include "WireFramePipeline.h"
#include "SkyBoxMesh.h"
#include "FrameBufferMesh.h"
#include "ForwardRenderingPipeline.h"
#include "ShadowRenderingPipeline.h"

class RendererBase
{
public:
	VkRenderPass RenderPass;
	std::vector<VkFramebuffer> SwapChainFramebuffers;

	ForwardRenderingPipeline forwardRendereringPipeline;
	SkyBoxPipeline skyboxPipeline;
	FrameBufferRenderingPipeline frameBufferPipeline;
	DebugLightRenderingPipeline DebugLightPipeline;
	CollisionDebugPipeline DebugCollisionPipeline;
	WireFramePipeline MeshviewPipeline;
	ShadowRenderingPipeline shadowPipeline;

	RendererBase();
	RendererBase(VulkanRenderer& renderer);
	~RendererBase();

	void Draw(VulkanRenderer& renderer, GraphicsPipeline pipeline, BaseMesh& mesh);
	void Draw(VulkanRenderer& renderer, GraphicsPipeline pipeline, std::vector<Mesh2>& MeshList);
	void ShadowDraw(VulkanRenderer& renderer, Mesh2& mesh);
	void Destroy(VulkanRenderer& renderer);
};

