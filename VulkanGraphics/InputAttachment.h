#pragma once
#include <vulkan\vulkan_core.h>
#include "Structs.h"

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
	VkFormat Format;
	unsigned int Width;
	unsigned int Height;


public:

	InputAttachment();
	InputAttachment(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	~InputAttachment();
	void CreateAttachmentImage(VkDevice device, VkPhysicalDevice physicalDevice, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void GetAttachmentTypeInfo(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType);
	void CreateAttachmentView(VkDevice device, VkImageAspectFlags aspectFlags);
	void ReCreateAttachment(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight);
	void DeleteInputAttachment(VkDevice device);

	VkImage AttachmentImage;
	VkDeviceMemory AttachmentImageMemory;
	VkImageView AttachmentImageView;
};

