#include "FrameBufferMesh.h"
#include <array>

FrameBufferMesh::FrameBufferMesh()
{
}

FrameBufferMesh::FrameBufferMesh(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkExtent2D swapChainExtent, VkRenderPass renderPass,
						 InputAttachment ColorAttachment, InputAttachment DepthAttachment, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize)
{
	CreateVertexBuffer(Device, PhysicalDevice, CommandPool, GraphicsQueue);
	CreateDescriptorPool(Device, SwapChainSize);
	CreateDescriptorSets(Device, ColorAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView, descriptorSetLayout, SwapChainSize);
}

FrameBufferMesh::~FrameBufferMesh()
{
}

void FrameBufferMesh::CreateVertexBuffer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue)
{
	VkDeviceSize bufferSize = sizeof(FrameBufferVertices[0]) * FrameBufferVertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(Device, PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, FrameBufferVertices.data(), (size_t)bufferSize);
	vkUnmapMemory(Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(Device, PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
	VulkanBufferManager::CopyBuffer(Device, PhysicalDevice, stagingBuffer, vertexBuffer, bufferSize, CommandPool, GraphicsQueue);

	vkDestroyBuffer(Device, stagingBuffer, nullptr);
	vkFreeMemory(Device, stagingBufferMemory, nullptr);
}

void FrameBufferMesh::CreateDescriptorPool(VkDevice Device, int SwapChainSize)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

	std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

	for (auto DescriptorPool : DescriptorPoolInfo)
	{
		VkDescriptorPoolSize DescriptorPoolBinding = {};
		DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
		DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(SwapChainSize);
		DescriptorPoolList.emplace_back(DescriptorPoolBinding);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
	poolInfo.pPoolSizes = DescriptorPoolList.data();
	poolInfo.maxSets = static_cast<uint32_t>(SwapChainSize);

	if (vkCreateDescriptorPool(Device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) 
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void FrameBufferMesh::CreateDescriptorSets(VkDevice Device, VkImageView ColorImageView, VkImageView DepthImageView, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize)
{
	std::vector<VkDescriptorSetLayout> layouts(SwapChainSize, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(SwapChainSize);
	if (vkAllocateDescriptorSets(Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	VkDescriptorImageInfo ColorImage = {};
	ColorImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	ColorImage.imageView = ColorImageView;
	ColorImage.sampler = VK_NULL_HANDLE;

	VkDescriptorImageInfo DepthImage = {};
	DepthImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DepthImage.imageView = DepthImageView;
	DepthImage.sampler = VK_NULL_HANDLE;

	for (size_t i = 0; i < SwapChainSize; i++)
	{
		std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pImageInfo = &ColorImage;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &DepthImage;

		vkUpdateDescriptorSets(Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void FrameBufferMesh::Draw(FrameBufferRenderingPipeline FrameBufferPipeline, VkCommandBuffer commandbuffer, int currentImage)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, FrameBufferPipeline.ShaderPipeline);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, FrameBufferPipeline.ShaderPipelineLayout, 0, 1, &descriptorSets[currentImage], 0, NULL);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdDraw(commandbuffer, 6, 1, 0, 0);
}

void FrameBufferMesh::RecreateSwapChainStage(VkDevice Device, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment,
										 InputAttachment DepthAttachment, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize)
{
	vkDestroyDescriptorPool(Device, descriptorPool, nullptr);
	CreateDescriptorPool(Device, SwapChainSize);
	CreateDescriptorSets(Device, ColorAttachment.AttachmentImageView, DepthAttachment.AttachmentImageView, descriptorSetLayout, SwapChainSize);
}

void FrameBufferMesh::Destory(VkDevice device)
{
	vkDestroyDescriptorPool(device, descriptorPool, nullptr);
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
}
