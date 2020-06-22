#include "BaseMesh.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(Renderer& renderer)
{
}

BaseMesh::BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices)
{
	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;

	CreateVertexBuffer(renderer);
}

BaseMesh::BaseMesh(Renderer& renderer, const TextureMaps& textureList)
{
	IndiceSize = 0;

	TextureList = textureList;
}

BaseMesh::BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;

	CreateVertexBuffer(renderer);
	CreateIndiceBuffer(renderer);
}

BaseMesh::BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const TextureMaps& textureList)
{
	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;
	TextureList = textureList;

	CreateVertexBuffer(renderer);
}

BaseMesh::BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const TextureMaps& textureList)
{
	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;
	TextureList = textureList;

	CreateVertexBuffer(renderer);
	CreateIndiceBuffer(renderer);
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::CreateVertexBuffer(Renderer& renderer)
{
	VkDeviceSize bufferSize = sizeof(VertexList[0]) * VertexList.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, VertexList.data(), (size_t)bufferSize);
	vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), stagingBuffer, vertexBuffer, bufferSize, *GetSecondaryCommandPool(renderer), *GetGraphicsQueue(renderer));

	vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);
}

void BaseMesh::CreateIndiceBuffer(Renderer& renderer)
{
	if (IndiceSize != 0)
	{
		VkDeviceSize bufferSize = sizeof(IndexList[0]) * IndexList.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*GetDevice(renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, IndexList.data(), (size_t)bufferSize);
		vkUnmapMemory(*GetDevice(renderer), stagingBufferMemory);

		VulkanBufferManager::CreateBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		VulkanBufferManager::CopyBuffer(*GetDevice(renderer), *GetPhysicalDevice(renderer), stagingBuffer, indexBuffer, bufferSize, *GetSecondaryCommandPool(renderer), *GetGraphicsQueue(renderer));

		vkDestroyBuffer(*GetDevice(renderer), stagingBuffer, nullptr);
		vkFreeMemory(*GetDevice(renderer), stagingBufferMemory, nullptr);
	}
}

void BaseMesh::CreateDescriptorPool(Renderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
{
	std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

	for (auto DescriptorPool : DescriptorPoolInfo)
	{
		VkDescriptorPoolSize DescriptorPoolBinding = {};
		DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
		DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(GetSwapChainImageCount(renderer));
		DescriptorPoolList.emplace_back(DescriptorPoolBinding);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
	poolInfo.pPoolSizes = DescriptorPoolList.data();
	poolInfo.maxSets = static_cast<uint32_t>(GetSwapChainImageCount(renderer));

	if (vkCreateDescriptorPool(*GetDevice(renderer), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void BaseMesh::CreateDescriptorSets(Renderer& renderer, VkDescriptorSetLayout layout)
{
	std::vector<VkDescriptorSetLayout> layouts(GetSwapChainImageCount(renderer), layout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(GetSwapChainImageCount(renderer));
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(GetSwapChainImageCount(renderer));
	if (vkAllocateDescriptorSets(*GetDevice(renderer), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void BaseMesh::CreateDescriptorSetsData(Renderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
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

	vkUpdateDescriptorSets(*GetDevice(renderer), static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
}

void BaseMesh::Destory(Renderer& renderer)
{
	if (VertexSize != 0)
	{
		vkDestroyDescriptorPool(*GetDevice(renderer), descriptorPool, nullptr);
		vkDestroyBuffer(*GetDevice(renderer), vertexBuffer, nullptr);
		vkFreeMemory(*GetDevice(renderer), vertexBufferMemory, nullptr);

		descriptorPool = VK_NULL_HANDLE;
		vertexBuffer = VK_NULL_HANDLE;
		vertexBufferMemory = VK_NULL_HANDLE;
	}

	if (IndiceSize != 0)
	{
		vkDestroyBuffer(*GetDevice(renderer), indexBuffer, nullptr);
		vkFreeMemory(*GetDevice(renderer), indexBufferMemory, nullptr);

		indexBuffer = VK_NULL_HANDLE;
		indexBufferMemory = VK_NULL_HANDLE;
	}
}
