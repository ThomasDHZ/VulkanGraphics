#pragma once
#include "InputAttachment.h"
#include "FramebufferPipeline.h"

class FrameBuffer : public BaseMesh
{
private:
	VulkanDevice DeviceInfo;



public:

	std::vector<VkBuffer> LightFragmentUniformBuffers;
	std::vector<VkDeviceMemory> LightFragmentUniformBuffersMemory;

	std::vector<VkBuffer> DebugUniformBuffers;
	std::vector<VkDeviceMemory> DebugBuffersMemory;

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(FramebufferPipeline pipeline, VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView);

	FrameBuffer();
	FrameBuffer(FramebufferPipeline pipeline, VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	~FrameBuffer();

	void UpdateUniformBuffer(LightingStruct ubo4, DebugStruct debug, int currentImage);
	void Draw(FramebufferPipeline pipeline, VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(FramebufferPipeline pipeline, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	void Destory();
};

