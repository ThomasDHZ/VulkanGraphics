#include "SkyBox.h"
#include <stb_image.h>
#include "VulkanBufferManager.h"
#include <vector>

SkyBox::SkyBox()
{
}

SkyBox::SkyBox(VulkanDevice deviceInfo)
{
	DeviceInfo = deviceInfo;
	SetUpVertexBuffer(deviceInfo);
}

SkyBox::~SkyBox()
{
}

void SkyBox::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(SkyBoxUniformBufferObject);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void SkyBox::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

	BaseMesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBox::CreateDescriptorSets(SkyBoxPipeline pipeline, CubeMapTexture cubeMapTexture)
{
	BaseMesh::CreateDescriptorSets(pipeline.ShaderPipelineDescriptorLayout);

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(SkyBoxUniformBufferObject);

		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = cubeMapTexture.textureImageView;
		imageInfo.sampler = cubeMapTexture.textureSampler;

		std::array<VkWriteDescriptorSet, 2>  descriptorWrites = {};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void SkyBox::SetUpVertexBuffer(VulkanDevice deviceInfo)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, vertexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void SkyBox::Draw(VkCommandBuffer commandbuffer, VkDescriptorSet descriptorset, SkyBoxPipeline pipeline)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipelineLayout, 0, 1, &descriptorset, 0, nullptr);
	vkCmdDraw(commandbuffer, vertices.size(), 1, 0, 0);
}

void SkyBox::Destory()
{
	vkDestroyBuffer(DeviceInfo.Device, vertexBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, vertexBufferMemory, nullptr);
}

