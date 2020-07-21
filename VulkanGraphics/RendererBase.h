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

class RendererBase
{
protected:
	std::vector<char> ReadFile(const std::string& filename);
	VkShaderModule CreateShaderModule(VkDevice Device, const std::vector<char>& code);

public:
	VkRenderPass RenderPass;
	VkDescriptorSetLayout DescriptorSetLayout;
	VkPipelineLayout RendererLayout;
	VkPipeline RendererPipeline;
	std::vector<VkFramebuffer> SwapChainFramebuffers;

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

