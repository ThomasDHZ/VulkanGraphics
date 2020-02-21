#pragma once
#include <vulkan\vulkan_core.h>
#include "Mesh.h"

enum AttachmentType
{
	VkColorAttachment,
	VkDepthAttachemnt
};

class InputAttachment
{
private:

	VulkanDevice DeviceInfo;
	VkFormat Format;
	unsigned int Width;
	unsigned int Height;

	void CreateAttachmentImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void CreateAttachmentView(VkImageAspectFlags aspectFlags);
public:

	InputAttachment();
	InputAttachment(VulkanDevice deviceInfo, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	~InputAttachment();

	VkImage AttachmentImage;
	VkDeviceMemory AttachmentImageMemory;
	VkImageView AttachmentImageView;
};

