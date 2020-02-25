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

	void GetAttachmentTypeInfo(AttachmentType attachmentType);
	void CreateAttachmentImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void CreateAttachmentView(VkImageAspectFlags aspectFlags);
public:

	InputAttachment();
	InputAttachment(VulkanDevice deviceInfo, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	~InputAttachment();

	void ReCreateAttachment(AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	void Destroy();

	VkImage AttachmentImage;
	VkDeviceMemory AttachmentImageMemory;
	VkImageView AttachmentImageView;
};

