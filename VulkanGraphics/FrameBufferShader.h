#pragma once
#include "BaseShader.h"
class FrameBufferShader : public BaseShader
{
private:
	void CreateDescriptorSetLayout();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkImageView ColorImageView, VkImageView DepthImageView);

public:
	FrameBufferShader();
	FrameBufferShader(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkImageView ColorImageView, VkImageView DepthImageView);
	~FrameBufferShader();

	void CreateShaderPipeLine(VkExtent2D swapChainExtent, VkRenderPass renderPass);
	void RecreateSwapChainInfo(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkImageView ColorImageView, VkImageView DepthImageView);
};

