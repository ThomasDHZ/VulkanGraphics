#pragma once
#include "FrameBufferShader.h"
#include "InputAttachment.h"

class FrameBuffer : public Mesh
{
private:
	VulkanDevice DeviceInfo;
	FrameBufferShader frameBufferShader;

	std::vector<VkBuffer> LightFragmentUniformBuffers;
	std::vector<VkDeviceMemory> LightFragmentUniformBuffersMemory;

	std::vector<VkBuffer> DebugUniformBuffers;
	std::vector<VkDeviceMemory> DebugBuffersMemory;

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView);

public:
	FrameBuffer();
	FrameBuffer(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	~FrameBuffer();

	void UpdateUniformBuffer(LightingStruct ubo4, DebugStruct debug, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	void DestroySwapChainStage();
	void Destory();
};

