#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "Vertex.h"
#include "Texture2D.h"

class BaseMesh : public VulkanResources
{
private:

protected:
	int VertexSize;
	uint16_t IndiceSize;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	void CreateVertexBuffer(VulkanRenderer& Renderer);
	void CreateIndiceBuffer(VulkanRenderer& Renderer);
	void CreateDescriptorPool(VulkanRenderer& Renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VulkanRenderer& Renderer, VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(VulkanRenderer& Renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:

	std::vector<Texture2D> TextureList;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	BaseMesh();
	BaseMesh(VulkanRenderer& Renderer);
	BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices);
	BaseMesh(VulkanRenderer& Renderer, const std::vector<Texture2D>& textureList);
	BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textureList);
	BaseMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	~BaseMesh();

	void UpdateUniformBuffer(VulkanRenderer& Renderer, VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	void Destory(VulkanRenderer& Renderer);
};

