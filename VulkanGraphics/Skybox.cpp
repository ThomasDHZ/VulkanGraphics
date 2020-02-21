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

void SkyBox::Draw(VkCommandBuffer commandbuffer, VkDescriptorSet descriptorset, VkPipeline pipeline, VkPipelineLayout pipelineLayout)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorset, 0, nullptr);
	vkCmdDraw(commandbuffer, vertices.size(), 1, 0, 0);
}

void SkyBox::DestorySwapChain(VkDevice device, int SwapChainSize)
{
	if (descriptorPool != nullptr)
	{
		vkDestroyDescriptorPool(device, descriptorPool, nullptr);
		descriptorPool = nullptr;
	}
}

void SkyBox::Destory(VkDevice device, int SwapChainSize)
{
	DestorySwapChain(device, SwapChainSize);

	vkDestroyDescriptorSetLayout(device, DescriptorSetLayout, nullptr);
	vkDestroyBuffer(device, vertexBuffer, nullptr);
	vkFreeMemory(device, vertexBufferMemory, nullptr);
}

