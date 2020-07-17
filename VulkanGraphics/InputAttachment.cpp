#include "InputAttachment.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

InputAttachment::InputAttachment()
{
}

InputAttachment::InputAttachment(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight)
{
	Width = WindowWidth;
	Height = WindowHeight;
	GetAttachmentTypeInfo(device, physicalDevice, attachmentType);
}

InputAttachment::~InputAttachment()
{
}

void InputAttachment::GetAttachmentTypeInfo(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType)
{
	switch (attachmentType)
	{
		case AttachmentType::VkColorAttachment:
		{
			Format = VK_FORMAT_R8G8B8A8_UNORM;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_COLOR_BIT);
			break;
		}
		case AttachmentType::VkHDRColorAttachment:
		{
			Format = VK_FORMAT_R32G32B32A32_SFLOAT;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_COLOR_BIT);
			break;
		}
		case AttachmentType::VkPositionAttachment:
		{
			Format = VK_FORMAT_R16G16B16A16_SFLOAT;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_COLOR_BIT);
			break;
		}
		case AttachmentType::VkNormalAttachment:
		{
			Format = VK_FORMAT_R16G16B16A16_SFLOAT;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_COLOR_BIT);
			break;
		}
		case AttachmentType::VkAlbedoAttachment:
		{
			Format = VK_FORMAT_R8G8B8A8_UNORM;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_COLOR_BIT);
			break;
		}
		case AttachmentType::VkDepthAttachemnt:
		{
			Format = VK_FORMAT_D32_SFLOAT;
			CreateAttachmentImage(device, physicalDevice, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			CreateAttachmentView(device, VK_IMAGE_ASPECT_DEPTH_BIT);
			break;
		}
	}
}

void InputAttachment::ReCreateAttachment(VkDevice device, VkPhysicalDevice physicalDevice, AttachmentType attachmentType, unsigned int WindowWidth, unsigned int WindowHeight)
{
	Width = WindowWidth;
	Height = WindowHeight;
	GetAttachmentTypeInfo(device, physicalDevice, attachmentType);
}

void InputAttachment::CreateAttachmentImage(VkDevice device, VkPhysicalDevice physicalDevice, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
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

	if (vkCreateImage(device, &imageInfo, nullptr, &AttachmentImage) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device, AttachmentImage, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = VulkanBufferManager::FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(device, &allocInfo, nullptr, &AttachmentImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(device, AttachmentImage, AttachmentImageMemory, 0);
}

void InputAttachment::CreateAttachmentView(VkDevice device, VkImageAspectFlags aspectFlags)
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

	if (vkCreateImageView(device, &viewInfo, nullptr, &AttachmentImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void InputAttachment::DeleteInputAttachment(VkDevice device)
{
	vkDestroyImageView(device, AttachmentImageView, nullptr);
	vkDestroyImage(device, AttachmentImage, nullptr);
	vkFreeMemory(device, AttachmentImageMemory, nullptr);

	AttachmentImageView = VK_NULL_HANDLE;
	AttachmentImage = VK_NULL_HANDLE;
	AttachmentImageMemory = VK_NULL_HANDLE;
}

