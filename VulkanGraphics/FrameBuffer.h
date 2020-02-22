#pragma once
#include "Mesh.h"
class FrameBuffer
{
private:
	VulkanDevice DeviceInfo;
public:
	FrameBuffer();
	FrameBuffer(VulkanDevice deviceInfo);
	~FrameBuffer();

	void CreateFrameBufferView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	void CreateFrameBuffer();
	void Draw(VkCommandBuffer commandbuffer, VkDescriptorSet descriptorset, VkPipeline pipeline, VkPipelineLayout pipelineLayout);
};

