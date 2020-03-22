#include "BaseMesh.h"
#include <stdexcept>

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo)
{
	DeviceInfo = deviceInfo;
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
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

void BaseMesh::CreateDescriptorSets(VkDescriptorSetLayout layout)
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

void BaseMesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList)
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


void BaseMesh::UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
{
	void* UniformData;
	vkMapMemory(DeviceInfo.Device, UniformBufferMemory, 0, UniformSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, UniformSize);
	vkUnmapMemory(DeviceInfo.Device, UniformBufferMemory);
}

void BaseMesh::Destory()
{
	if (IndiceSize > 0)
	{
		vkDestroyBuffer(DeviceInfo.Device, indexBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, indexBufferMemory, nullptr);
	}

	if (VertexSize >= 0)
	{
		vkDestroyBuffer(DeviceInfo.Device, vertexBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, vertexBufferMemory, nullptr);
	}
}
