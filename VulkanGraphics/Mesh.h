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
#include "AmbientLight.h"
#include "UniformBuffer.h"

struct PositionMatrix
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct DirectionalLight
{
	alignas(16) glm::vec3 Direction;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
};

struct Material
{
	alignas(16) glm::vec3 ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
	alignas(4)  float Shininess;
};

struct MeshProp
{
	DirectionalLight light;
	Material material;
	alignas(16) glm::vec3 viewPos;
};

class Mesh : public BaseMesh
{
private:

protected:
	void CreateUniformBuffers(VulkanRenderer& Renderer);
	void CreateDescriptorPool(VulkanRenderer& Renderer);
	void CreateDescriptorSets(VulkanRenderer& Renderer);

public:

	std::string MeshName;

	UniformBuffer PositionMatrixBuffer;
	UniformBuffer ViewPosBuffer;

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	Mesh();
	Mesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	Mesh(VulkanRenderer& Renderer, const std::vector<Texture2D>& textureList);
	~Mesh();

	void Draw(VulkanRenderer& Renderer, int currentFrame);
	void UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix, MeshProp viewpos);
	void Destroy(VulkanRenderer& Renderer);
};

