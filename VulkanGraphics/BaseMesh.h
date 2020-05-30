#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "Vertex.h"
#include "Texture2D.h"

struct PositionMatrix
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class BaseMesh : public VulkanResources
{
private:

protected:
	int VertexSize;
	uint16_t IndiceSize;

	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	void CreateVertexBuffer(Renderer& renderer);
	void CreateIndiceBuffer(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(Renderer& renderer, VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(Renderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:

	std::vector<Texture2D> TextureList;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	BaseMesh();
	BaseMesh(Renderer& renderer);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices);
	BaseMesh(Renderer& renderer, const std::vector<Texture2D>& textureList);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<Texture2D>& textureList);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	~BaseMesh();

	void Destory(Renderer& renderer);
};

