#include "BaseMesh.h"
#include <stdexcept>
#include "VulkanBufferManager.h"

BaseMesh::BaseMesh()
{
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer)
{
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices)
{
	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;

	CreateVertexBuffer(Renderer);
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer, const std::vector<Texture2D>& textureList)
{
	IndiceSize = 0;

	TextureList = textureList;
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
{
	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;

	CreateVertexBuffer(Renderer);
	CreateIndiceBuffer(Renderer);
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textureList)
{
	VertexSize = vertices.size();
	IndiceSize = 0;

	VertexList = vertices;
	TextureList = textureList;

	CreateVertexBuffer(Renderer);
}

BaseMesh::BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList)
{
	VertexSize = vertices.size();
	IndiceSize = indices.size();

	VertexList = vertices;
	IndexList = indices;
	TextureList = textureList;

	CreateVertexBuffer(Renderer);
	CreateIndiceBuffer(Renderer);
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::CreateVertexBuffer(VulkanRenderer& Renderer)
{
	VkDeviceSize bufferSize = sizeof(VertexList[0]) * VertexList.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, VertexList.data(), (size_t)bufferSize);
	vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

	VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

	VulkanBufferManager::CopyBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), stagingBuffer, vertexBuffer, bufferSize, Renderer.MainCommandPool, *GetGraphicsQueue(Renderer));

	vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
	vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);
}

void BaseMesh::CreateIndiceBuffer(VulkanRenderer& Renderer)
{
	if (IndiceSize != 0)
	{
		VkDeviceSize bufferSize = sizeof(IndexList[0]) * IndexList.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(*GetDevice(Renderer), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, IndexList.data(), (size_t)bufferSize);
		vkUnmapMemory(*GetDevice(Renderer), stagingBufferMemory);

		VulkanBufferManager::CreateBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		VulkanBufferManager::CopyBuffer(*GetDevice(Renderer), *GetPhysicalDevice(Renderer), stagingBuffer, indexBuffer, bufferSize, Renderer.MainCommandPool, *GetGraphicsQueue(Renderer));

		vkDestroyBuffer(*GetDevice(Renderer), stagingBuffer, nullptr);
		vkFreeMemory(*GetDevice(Renderer), stagingBufferMemory, nullptr);
	}
}

void BaseMesh::CreateDescriptorPool(VulkanRenderer& Renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
{
	std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

	for (auto DescriptorPool : DescriptorPoolInfo)
	{
		VkDescriptorPoolSize DescriptorPoolBinding = {};
		DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
		DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(GetSwapChainImageCount(Renderer));
		DescriptorPoolList.emplace_back(DescriptorPoolBinding);
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
	poolInfo.pPoolSizes = DescriptorPoolList.data();
	poolInfo.maxSets = static_cast<uint32_t>(GetSwapChainImageCount(Renderer));

	if (vkCreateDescriptorPool(*GetDevice(Renderer), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void BaseMesh::CreateDescriptorSets(VulkanRenderer& Renderer, VkDescriptorSetLayout layout)
{
	std::vector<VkDescriptorSetLayout> layouts(GetSwapChainImageCount(Renderer), layout);
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(GetSwapChainImageCount(Renderer));
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(GetSwapChainImageCount(Renderer));
	if (vkAllocateDescriptorSets(*GetDevice(Renderer), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
}

void BaseMesh::CreateDescriptorSetsData(VulkanRenderer& Renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
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

	vkUpdateDescriptorSets(*GetDevice(Renderer), static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
}

void BaseMesh::UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize)
{
	void* UniformData;
	vkMapMemory(*GetDevice(Renderer), UniformBufferMemory, 0, UniformSize, 0, &UniformData);
	memcpy(UniformData, UniformObjectData, UniformSize);
	vkUnmapMemory(*GetDevice(Renderer), UniformBufferMemory);
}

void BaseMesh::Destory(VulkanRenderer& Renderer)
{
	if (VertexSize != 0)
	{
		vkDestroyDescriptorPool(*GetDevice(Renderer), descriptorPool, nullptr);
		vkDestroyBuffer(*GetDevice(Renderer), vertexBuffer, nullptr);
		vkFreeMemory(*GetDevice(Renderer), vertexBufferMemory, nullptr);

		descriptorPool = VK_NULL_HANDLE;
		vertexBuffer = VK_NULL_HANDLE;
		vertexBufferMemory = VK_NULL_HANDLE;
	}

	if (IndiceSize != 0)
	{
		vkDestroyBuffer(*GetDevice(Renderer), indexBuffer, nullptr);
		vkFreeMemory(*GetDevice(Renderer), indexBufferMemory, nullptr);

		indexBuffer = VK_NULL_HANDLE;
		indexBufferMemory = VK_NULL_HANDLE;
	}
}
