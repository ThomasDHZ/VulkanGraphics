#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "Vertex.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"

struct PositionMatrix
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
	alignas(4) float timer;
};

struct TextureFlags
{
	alignas(4) int DiffuseMapFlag;
	alignas(4) int SpecularMapFlag;
	alignas(4) int NormalMapFlag;
	alignas(4) int DisplacementMapFlag;
	alignas(4) int AlphaMapFlag;
	alignas(4) int CubeMapFlag;
};

struct TextureMaps
{
	Texture2D DiffuseMap;
	Texture2D SpecularMap;
	Texture2D NormalMap;
	Texture2D DisplacementMap;
	Texture2D AlphaMap;
	CubeMapTexture CubeMap;
};

class BaseMesh : public VulkanResources
{
private:

protected:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;



	int VertexSize;
	uint16_t IndiceSize;

	void CreateVertexBuffer(Renderer& renderer);
	void CreateIndiceBuffer(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(Renderer& renderer, VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(Renderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:


	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;

	TextureMaps TextureList;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;

	BaseMesh();
	BaseMesh(Renderer& renderer);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices);
	BaseMesh(Renderer& renderer, const TextureMaps& textureList);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const TextureMaps& textureList);
	BaseMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const TextureMaps& textureList);
	~BaseMesh();

	void Destory(Renderer& renderer);
};

