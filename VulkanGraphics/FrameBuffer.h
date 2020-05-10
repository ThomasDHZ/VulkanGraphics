#pragma once
#include "FrameBufferShader.h"
#include "InputAttachment.h"

class FrameBuffer
{
private:


public:
	FrameBuffer();
	FrameBuffer(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment, InputAttachment DepthAttachment);
	~FrameBuffer();

	VulkanDevice DeviceInfo;
	FrameBufferShader frameBufferShader;

	void CreateFrameBufferView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateFrameBuffer();
	void Draw(VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment, InputAttachment DepthAttachment);
	void DestroySwapChainStage();
	void Destory();
};

