#pragma once
#include "FrameBufferShader.h"
#include "InputAttachment.h"

class FrameBuffer
{
private:
	VulkanDevice DeviceInfo;
	FrameBufferShader frameBufferShader;

public:
	FrameBuffer();
	FrameBuffer(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	~FrameBuffer();

	void CreateFrameBufferView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateFrameBuffer();
	void UpdateUniformBuffer(LightingStruct ubo4, DebugStruct debug, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment);
	void DestroySwapChainStage();
	void Destory();
};

