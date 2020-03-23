#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VulkanBufferManager.h"
#include "BaseMesh.h"
#include "Texture2D.h"
#include "Vertex.h"

struct Light
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Color;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
	alignas(4) float Radius;
};

struct LightingStruct
{
	Light light;
	alignas(16) glm::vec3 viewPos;
	alignas(16) float shininess;
};

struct DebugStruct
{
	alignas(4) int DebugLayer;
};

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class Mesh : public BaseMesh
{
private:
	std::vector<Vertex> VertexList;
	std::vector<uint32_t> IndexList;

	void CreateVertexBuffer();
	void CreateIndiceBuffer();

	void CreateUniformBuffers();


public:

	std::vector<Texture2D> TextureList;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	Mesh();
	Mesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture2D>& textureList);
	~Mesh();

	void UpdateTextures(const std::vector<Texture2D>& textureList);
	void CreateDescriptorPool();
	void CreateDescriptorSets();
	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage);
	void UpdateSwapChain();
	void ClearSwapChain();
	void Destory();
};

