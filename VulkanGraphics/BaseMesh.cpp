#include "BaseMesh.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo)
{
	DeviceInfo = deviceInfo;
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices)
{
	DeviceInfo = deviceInfo;

	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;

	CreateVertexBuffer();
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo, const std::vector<Texture2D>& textureList)
{
	DeviceInfo = deviceInfo;

	IndiceSize = 0;

	TextureList = textureList;
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	DeviceInfo = deviceInfo;

	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;

	CreateVertexBuffer();
	CreateIndiceBuffer();
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textureList)
{
	DeviceInfo = deviceInfo;

	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;
	TextureList = textureList;

	CreateVertexBuffer();
}

BaseMesh::BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList)
{
	DeviceInfo = deviceInfo;

	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;
	TextureList = textureList;

	CreateVertexBuffer();
	CreateIndiceBuffer();
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::CreateVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(VertexList[0]) * VertexList.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, VertexList.data(), (size_t)bufferSize);
	vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, vertexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

	vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
	vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
}

void BaseMesh::CreateIndiceBuffer()
{
	if (IndiceSize != 0)
	{
		VkDeviceSize bufferSize = sizeof(IndexList[0]) * IndexList.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(DeviceInfo.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, IndexList.data(), (size_t)bufferSize);
		vkUnmapMemory(DeviceInfo.Device, stagingBufferMemory);

		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		VulkanBufferManager::CopyBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, stagingBuffer, indexBuffer, bufferSize, DeviceInfo.CommandPool, DeviceInfo.GraphicsQueue);

		vkDestroyBuffer(DeviceInfo.Device, stagingBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, stagingBufferMemory, nullptr);
	}
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
	if (indexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);
		descriptorPool = VK_NULL_HANDLE;
	}

	if (indexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(DeviceInfo.Device, indexBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, indexBufferMemory, nullptr);

		indexBuffer = VK_NULL_HANDLE;
		indexBufferMemory = VK_NULL_HANDLE;
	}

	if (vertexBuffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(DeviceInfo.Device, vertexBuffer, nullptr);
		vkFreeMemory(DeviceInfo.Device, vertexBufferMemory, nullptr);

		vertexBuffer = VK_NULL_HANDLE;
		vertexBufferMemory = VK_NULL_HANDLE;
	}
}
