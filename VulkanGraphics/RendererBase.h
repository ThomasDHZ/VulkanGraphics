#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "VulkanRenderer.h"
#include "Mesh.h"
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

	RendererBase();
	RendererBase(VulkanRenderer& renderer);
	~RendererBase();

	void Draw(VulkanRenderer& renderer, GraphicsPipeline pipeline, BaseMesh& mesh);
	void Draw(VulkanRenderer& renderer, GraphicsPipeline pipeline, std::vector<Mesh>& MeshList);
	void Destroy(VulkanRenderer& renderer);
};

