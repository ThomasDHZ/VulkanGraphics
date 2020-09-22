#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "VulkanEngine.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"
#include "Rendering2DPipeline.h"
#include "Reflection2DPipeline.h"
#include "UnderWater2DPipeline.h"
#include "CollisionDebugPipeline.h"
#include "ForwardRenderingPipeline.h"
#include "SkyBoxPipeline.h"
#include "DebugLightRenderingPipeline.h"
#include "WireFramePipeline.h"
#include "FrameBufferRenderingPipeline.h"

struct RendererDrawMessage
{
	unsigned int RendererID;
	VertexBuffer MeshVertex;
	IndicesBuffer MeshIndices;
	std::vector<VkDescriptorSet> DescriptorSets;
	std::shared_ptr<GraphicsPipeline> pipeline;
};

class RendererBase
{
protected:
	std::vector<VkPipelineColorBlendAttachmentState> ColorBlendAttachment;
	VkPipelineColorBlendStateCreateInfo ColorBlendingSettings;

	virtual void SetUpColorBlendingSettings();
public:
	VkRenderPass RenderPass;
	std::vector<VkFramebuffer> SwapChainFramebuffers;

	RendererBase();
	RendererBase(VulkanEngine& renderer);
	~RendererBase();

	void Draw(VulkanEngine& renderer, std::shared_ptr<RendererDrawMessage>& drawMessage) const;
	virtual void Destroy(VulkanEngine& renderer);
};

