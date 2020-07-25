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

	RendererBase();
	RendererBase(VulkanRenderer& renderer);
	~RendererBase();

	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, FrameBufferMesh& mesh);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame,  Mesh2& mesh);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, SkyBoxMesh mesh);
	void Destroy(VkDevice Device);
};

