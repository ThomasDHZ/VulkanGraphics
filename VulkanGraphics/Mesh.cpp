#include "Mesh.h"
#include <stdexcept>

Mesh::Mesh()
{
}

Mesh::Mesh(VulkanDevice deviceInfo)
{
	DeviceInfo = deviceInfo;
}

Mesh::~Mesh()
{
}

void Mesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
{
	std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

	for (auto DescriptorPool : DescriptorPoolInfo)
	{
		VkDescriptorPoolSize DescriptorPoolBinding = {};
		DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
		DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(DeviceInfo.SwapChainSize);
		DescriptorPoolList.emplace_back(DescriptorPoolBinding);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
	poolInfo.pPoolSizes = DescriptorPoolList.data();
	poolInfo.maxSets = static_cast<uint32_t>(DeviceInfo.SwapChainSize);

	if (vkCreateDescriptorPool(DeviceInfo.Device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Mesh::CreateDescriptorSets(VkDescriptorSetLayout layout)
{
	std::vector<VkDescriptorSetLayout> layouts(DeviceInfo.SwapChainSize, layout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(DeviceInfo.SwapChainSize);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(DeviceInfo.SwapChainSize);
	if (vkAllocateDescriptorSets(DeviceInfo.Device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void Mesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList)
{
	std::vector<VkWriteDescriptorSet>  WriteDescriptorInfo = {};

	for (int x = 0; x < descriptorWritesList.size(); x++)
	{
		VkWriteDescriptorSet DescriptorSet = {};
		DescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		DescriptorSet.dstSet = descriptorWritesList[x].DstSet;
		DescriptorSet.dstBinding = descriptorWritesList[x].DstBinding;
		DescriptorSet.dstArrayElement = 0;
		DescriptorSet.descriptorType = descriptorWritesList[x].DescriptorType;
		DescriptorSet.descriptorCount = 1;
		if (descriptorWritesList[x].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
		{
			DescriptorSet.pBufferInfo = &descriptorWritesList[x].DescriptorBufferInfo;
		}
		else
		{
			DescriptorSet.pImageInfo = &descriptorWritesList[x].DescriptorImageInfo;
		}
		WriteDescriptorInfo.emplace_back(DescriptorSet);
	}

	vkUpdateDescriptorSets(DeviceInfo.Device, static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
}


void Mesh::UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
{
	void* UniformData;
	vkMapMemory(DeviceInfo.Device, UniformBufferMemory, 0, UniformSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, UniformSize);
	vkUnmapMemory(DeviceInfo.Device, UniformBufferMemory);
}

void Mesh::Draw(VkCommandBuffer commandbuffer, VkPipeline pipeline, VkPipelineLayout layout, int currentImage)
{
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindPipeline(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	vkCmdBindVertexBuffers(commandbuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &descriptorSets[currentImage], 0, nullptr);
	vkCmdDraw(commandbuffer, VerticeSize, 1, 0, 0);
	//vkCmdBindIndexBuffer(commandbuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
	//vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
}

void Mesh::Destory()
{
	DestorySwapChain();
}

void Mesh::DestorySwapChain()
{
	for (int x = 0; x < DeviceInfo.SwapChainSize; x++)
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[x], nullptr);
		vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[x], nullptr);
	}

	vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);
}
