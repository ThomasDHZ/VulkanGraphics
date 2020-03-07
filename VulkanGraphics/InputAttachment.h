#pragma once
#include <vulkan\vulkan_core.h>
#include "Mesh.h"

enum AttachmentType
{
	VkColorAttachment,
	VkPositionAttachment,
	VkNormalAttachment,
	VkAlbedoAttachment,
	VkDepthAttachemnt
};

class InputAttachment
{
private:

	VulkanDevice DeviceInfo;
	VkFormat Format;
	unsigned int Width;
	unsigned int Height;


public:

	InputAttachment();
	InputAttachment(VulkanDevice deviceInfo, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	~InputAttachment();
	void CreateAttachmentImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void GetAttachmentTypeInfo(AttachmentType attachmentType);
	void CreateAttachmentView(VkImageAspectFlags aspectFlags);
	void ReCreateAttachment(AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	void ResizeFrameBuffer();

	VkImage AttachmentImage;
	VkDeviceMemory AttachmentImageMemory;
	VkImageView AttachmentImageView;
};

