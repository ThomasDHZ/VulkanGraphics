#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Mesh2.h"

class RendererBase2
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

	RendererBase2();
	~RendererBase2();

	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, Mesh2& mesh);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList);
};

