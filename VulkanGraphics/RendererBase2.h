#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>

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
};

