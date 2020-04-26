#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "Vertex.h"
#include "Texture2D.h"

class BaseMesh
{
private:
	void CreateVertexBuffer();
	void CreateIndiceBuffer();

protected:
	VulkanDevice DeviceInfo;

	int VertexSize;
	uint16_t IndiceSize;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:

	std::vector<Texture2D> TextureList;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	BaseMesh();
	BaseMesh(VulkanDevice deviceInfo);
	BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices);
	BaseMesh(VulkanDevice deviceInfo, const std::vector<Texture2D>& textureList);
	BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textureList);
	BaseMesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	~BaseMesh();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	void Destory();
};

