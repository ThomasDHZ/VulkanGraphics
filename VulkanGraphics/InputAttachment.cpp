#include "InputAttachment.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

InputAttachment::InputAttachment()
{
}

InputAttachment::InputAttachment(VulkanDevice deviceInfo, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight)
{
	DeviceInfo = deviceInfo;
	Width = WindowWidth;
	Height = WindowHeight;
	GetAttachmentTypeInfo(attachmentType);
}

InputAttachment::~InputAttachment()
{
}

void InputAttachment::GetAttachmentTypeInfo(AttachmentType attachmentType)
{
	switch (attachmentType)
	{
	case AttachmentType::VkColorAttachment:
	{
		Format = VK_FORMAT_R8G8B8A8_UNORM;
		CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);
		break;
	}
	case AttachmentType::VkPositionAttachment:
	{
		Format = VK_FORMAT_R16G16B16A16_SFLOAT;
		CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);
		break;
	}
	case AttachmentType::VkNormalAttachment:
	{
		Format = VK_FORMAT_R16G16B16A16_SFLOAT;
		CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);
		break;
	}
	case AttachmentType::VkAlbedoAttachment:
	{
		Format = VK_FORMAT_R8G8B8A8_UNORM;
		CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CreateAttachmentView(VK_IMAGE_ASPECT_COLOR_BIT);
		break;
	}
	case AttachmentType::VkDepthAttachemnt:
	{
		Format = VK_FORMAT_D32_SFLOAT_S8_UINT;
		CreateAttachmentImage(VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		CreateAttachmentView(VK_IMAGE_ASPECT_DEPTH_BIT);
		break;
	}
	}
}

void InputAttachment::ReCreateAttachment(AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight)
{
	Width = WindowWidth;
	Height = WindowHeight;
	GetAttachmentTypeInfo(attachmentType);
}

void InputAttachment::CreateAttachmentImage(VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.format = Format;
	imageInfo.extent.width = Width;
	imageInfo.extent.height = Height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = usage | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (vkCreateImage(DeviceInfo.Device, &imageInfo, nullptr, &AttachmentImage) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(DeviceInfo.Device, AttachmentImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(DeviceInfo.PhysicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(DeviceInfo.Device, &allocInfo, nullptr, &AttachmentImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(DeviceInfo.Device, AttachmentImage, AttachmentImageMemory, 0);
}

void InputAttachment::CreateAttachmentView(VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = Format;
	viewInfo.subresourceRange = {};
	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;
	viewInfo.image = AttachmentImage;

	if (vkCreateImageView(DeviceInfo.Device, &viewInfo, nullptr, &AttachmentImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void InputAttachment::Destroy()
{
	vkDestroyImageView(DeviceInfo.Device, AttachmentImageView, nullptr);
	vkDestroyImage(DeviceInfo.Device, AttachmentImage, nullptr);
	vkFreeMemory(DeviceInfo.Device, AttachmentImageMemory, nullptr);
}

