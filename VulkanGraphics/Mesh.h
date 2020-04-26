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

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets();

public:
	std::string MeshName;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	Mesh();
	Mesh(VulkanDevice deviceInfo, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	~Mesh();

	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentFrame);
	void SecBufferDraw(VkCommandBuffer& commandbuffer, VkCommandBufferBeginInfo cmdInfo, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentFrame);
	void UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage);
	void UpdateSwapChain();
	void Destroy();
};

