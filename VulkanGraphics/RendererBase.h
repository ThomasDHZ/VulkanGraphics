#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "VulkanRenderer.h"
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
	std::vector<VkFramebuffer> SwapChainFramebuffers;


	RendererBase();
	~RendererBase();

	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, Mesh2& mesh);
	void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, std::vector<Mesh2>& MeshList);
	//void Draw(VkExtent2D extent, VkCommandBuffer commandBuffer, int frame, SkyBoxMesh mesh);
	void Destroy(VkDevice Device);
};

