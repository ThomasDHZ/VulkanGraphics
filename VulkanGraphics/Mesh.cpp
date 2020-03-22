#include "Mesh.h"

Mesh::Mesh() : BaseMesh()
{
}

Mesh::Mesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertexList, const std::vector<uint32_t>& indexList, const std::vector<Texture2D>& textureList) : BaseMesh(deviceInfo)
{
	VertexSize = vertexList.size();
	IndiceSize = indexList.size();

	VertexList = vertexList;
	IndexList = indexList;
	TextureList = textureList;

	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
	CreateVertexBuffer();
	CreateIndiceBuffer();
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer()
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

void Mesh::CreateIndiceBuffer()
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

void Mesh::CreateUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	uniformBuffers.resize(DeviceInfo.SwapChainSize);
	uniformBuffersMemory.resize(DeviceInfo.SwapChainSize);
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VulkanBufferManager::CreateBuffer(DeviceInfo.Device, DeviceInfo.PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
	}
}

void Mesh::UpdateTextures(const std::vector<Texture2D>& textureList)
{
	TextureList.clear();
	TextureList = textureList;
}

void Mesh::CreateDescriptorPool()
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	/*DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;*/

	BaseMesh::CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh::CreateDescriptorSets()
{
	BaseMesh::CreateDescriptorSets(DeviceInfo.descriptorSetLayout);

	VkDescriptorImageInfo DiffuseMap = {};
	DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	DiffuseMap.imageView = TextureList[0].textureImageView;
	DiffuseMap.sampler = TextureList[0].textureSampler;

	VkDescriptorImageInfo SpecularMap = {};
	SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	SpecularMap.imageView = TextureList[1].textureImageView;
	SpecularMap.sampler = TextureList[1].textureSampler;

	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		std::array<WriteDescriptorSetInfo, 2>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = bufferInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		WriteDescriptorInfo[1].DescriptorImageInfo = DiffuseMap;

		//WriteDescriptorInfo[2].DstBinding = 2;
		//WriteDescriptorInfo[2].DstSet = descriptorSets[i];
		//WriteDescriptorInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		//WriteDescriptorInfo[2].DescriptorImageInfo = SpecularMap;

		Mesh::CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void Mesh::UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage)
{
	BaseMesh::UpdateUniformBuffer(uniformBuffersMemory[currentImage], static_cast<void*>(&ubo2), sizeof(ubo2));
}

void Mesh::RecreateSwapChainStage()
{
	CreateUniformBuffers();
	CreateDescriptorPool();
	CreateDescriptorSets();
}

void Mesh::ClearSwapChain()
{
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[i], nullptr);
		vkFreeMemory(DeviceInfo.Device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);
}
void Mesh::Destory()
{
	for (size_t i = 0; i < DeviceInfo.SwapChainSize; i++)
	{
		vkDestroyBuffer(DeviceInfo.Device, uniformBuffers[i], nullptr);
		vkFreeMemory(DeviceInfo.Device,uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorPool(DeviceInfo.Device, descriptorPool, nullptr);

	BaseMesh::Destory();
}
