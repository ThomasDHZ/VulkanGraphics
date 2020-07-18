#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Mesh2.h"

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
	std::vector<VkFramebuffer> swapChainFramebuffers;

	RendererBase();
	~RendererBase();

	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, Mesh2& mesh);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList);
};

