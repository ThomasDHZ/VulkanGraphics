#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() : BaseMesh()
{
}

FrameBuffer::FrameBuffer(FramebufferPipeline pipeline, VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment) : BaseMesh(deviceInfo)
{
	DeviceInfo = deviceInfo;

	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(pipeline, PositionAttachment.AttachmentImageView, NormalAttachment.AttachmentImageView, AlbedoAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView);
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(LightingStruct);

	LightFragmentUniformBuffers.resize(DeviceInfo.SwapChainSize);
	LightFragmentUniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, LightFragmentUniformBuffers[i], LightFragmentUniformBuffersMemory[i]);
	}

	VkDeviceSize debugbufferSize = sizeof(DebugStruct);

	DebugUniformBuffers.resize(DeviceInfo.SwapChainSize);
	DebugBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, debugbufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, DebugUniformBuffers[i], DebugBuffersMemory[i]);
	}
}

void FrameBuffer::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 6>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void FrameBuffer::CreateDescriptorSets(FramebufferPipeline pipeline, VkImageView PositionImageView, VkImageView NormalImageView, VkImageView AlbedoImageView, VkImageView DepthImageView)
{
	BaseMesh::CreateDescriptorSets(pipeline.ShaderPipelineDescriptorLayout);

	VkDescriptorImageInfo ColorImage = {};
	ColorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	ColorImage.imageView = PositionImageView;
	ColorImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo NormalImage = {};
	NormalImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	NormalImage.imageView = NormalImageView;
	NormalImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo AlbedoImage = {};
	AlbedoImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	AlbedoImage.imageView = AlbedoImageView;
	AlbedoImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo DepthImage = {};
	DepthImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DepthImage.imageView = DepthImageView;
	DepthImage.sampler = VK_NULL_HANDLE;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = LightFragmentUniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(LightingStruct);

		VkDescriptorBufferInfo DebugInfo = {};
		DebugInfo.buffer = DebugUniformBuffers[i];
		DebugInfo.offset = 0;
		DebugInfo.range = sizeof(DebugStruct);

		std::array<WriteDescriptorSetInfo, 6>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[0].DescriptorImageInfo = ColorImage;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[1].DescriptorImageInfo = NormalImage;

		WriteDescriptorInfo[2].DstBinding = 2;
		WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[2].DescriptorImageInfo = AlbedoImage;

		WriteDescriptorInfo[3].DstBinding = 3;
		WriteDescriptorInfo[3].DstSet = descriptorSets[i];
		WriteDescriptorInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		WriteDescriptorInfo[3].DescriptorImageInfo = DepthImage;

		WriteDescriptorInfo[4].DstBinding = 4;
		WriteDescriptorInfo[4].DstSet = descriptorSets[i];
		WriteDescriptorInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[4].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[5].DstBinding = 5;
		WriteDescriptorInfo[5].DstSet = descriptorSets[i];
		WriteDescriptorInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[5].DescriptorBufferInfo = DebugInfo;

		BaseMesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void FrameBuffer::UpdateUniformBuffer(LightingStruct ubo4, DebugStruct debug, int currentImage)
{
	BaseMesh::UpdateUniformBuffer(LightFragmentUniformBuffersMemory[currentImage], static_cast<void*>(&ubo4), sizeof(ubo4));
	BaseMesh::UpdateUniformBuffer(DebugBuffersMemory[currentImage], static_cast<void*>(&debug), sizeof(debug));
}

void FrameBuffer::Draw(FramebufferPipeline pipeline, VkCommandBuffer commandbuffer, int currentImage)
{
	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipelineLayout, 0, 1, &descriptorSets[currentImage], 0, NULL);
	vkCmdDraw(commandbuffer, 3, 1, 0, 0);
}

void FrameBuffer::RecreateSwapChainStage(FramebufferPipeline pipeline, InputAttachment PositionAttachment, InputAttachment NormalAttachment, InputAttachment AlbedoAttachment, InputAttachment DepthAttachment)
{
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets(pipeline, PositionAttachment.AttachmentImageView, NormalAttachment.AttachmentImageView, AlbedoAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView);
}

void FrameBuffer::Destory()
{
	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		vkDestroyBuffer(DeviceInfo.Device, LightFragmentUniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, LightFragmentUniformBuffersMemory[x], nullptr);
	}

	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		vkDestroyBuffer(DeviceInfo.Device, DebugUniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, DebugBuffersMemory[x], nullptr);
	}

	BaseMesh::Destory();
}
