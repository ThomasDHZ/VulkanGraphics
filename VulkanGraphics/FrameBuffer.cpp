#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment)
{
	DeviceInfo = deviceInfo;
	frameBufferShader = FrameBufferShader(DeviceInfo, swapChainExtent, renderPass, PositionAttachment.AttachmentImageView, NormalAttachment.AttachmentImageView, AlbedoAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView);
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::CreateFrameBufferView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	//swapChainImageViews.resize(DeviceInfo.SwapChainSize);

	//for (uint32_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	//{
	//	VkImageViewCreateInfo viewInfo = {};
	//	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	//	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	//	viewInfo.format = format;
	//	viewInfo.subresourceRange = {};
	//	viewInfo.subresourceRange.aspectMask = aspectFlags;
	//	viewInfo.subresourceRange.baseMipLevel = 0;
	//	viewInfo.subresourceRange.levelCount = 1;
	//	viewInfo.subresourceRange.baseArrayLayer = 0;
	//	viewInfo.subresourceRange.layerCount = 1;
	//	viewInfo.image = image;

	//	if (vkCreateImageView(DeviceInfo.Device, &viewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS)
	//	{
	//		throw std::runtime_error("failed to create texture image view!");
	//	}
	//}
}

void FrameBuffer::CreateFrameBuffer()
{
	//swapChainFramebuffers.resize(swapChainImageViews.size());

	//for (size_t i = 0; i < swapChainImageViews.size(); i++)
	//{
	//	std::array<VkImageView, 3> attachments = {
	//		swapChainImageViews[i],
	//		ColorAttachment.AttachmentImageView,
	//		DepthAttachment.AttachmentImageView
	//	};

	//	VkFramebufferCreateInfo framebufferInfo = {};
	//	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	//	framebufferInfo.renderPass = renderPass;
	//	framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	//	framebufferInfo.pAttachments = attachments.data();
	//	framebufferInfo.width = swapChainExtent.width;
	//	framebufferInfo.height = swapChainExtent.height;
	//	framebufferInfo.layers = 1;

	//	if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
	//		throw std::runtime_error("failed to create framebuffer!");
	//	}
	//}
}

void FrameBuffer::Draw(VkCommandBuffer commandbuffer, int currentImage)
{
	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferShader.ShaderPipeline);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, frameBufferShader.ShaderPipelineLayout, 0, 1, &frameBufferShader.descriptorSets[currentImage], 0, NULL);
	vkCmdDraw(commandbuffer, 3, 1, 0, 0);
}

void FrameBuffer::RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment)
{
	frameBufferShader.RecreateSwapChainInfo(swapChainExtent, renderPass, PositionAttachment.AttachmentImageView, NormalAttachment.AttachmentImageView, AlbedoAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView);
}

void FrameBuffer::DestroySwapChainStage()
{
	frameBufferShader.DestorySwapChain();
}

void FrameBuffer::Destory()
{
	frameBufferShader.Destory();
}
