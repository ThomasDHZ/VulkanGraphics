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
#include "UniformBuffer.h"
#include "LightStructs.h"

struct Material
{
	alignas(16) glm::vec3 ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
	alignas(4)  float Shininess;
};

struct Lightingz
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Color;
};

struct MeshProp
{
	DirectionalLightBuffer directionalLight;
	PointLightBuffer pointLight;
	SpotLightBuffer spotLight;
	Material material;
	alignas(16) glm::vec3 viewPos;
	alignas(8) glm::vec2 SpriteUV;
};

class Mesh : public BaseMesh
{
private:

protected:
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);

public:

	std::string MeshName;

	UniformBuffer PositionMatrixBuffer;
	UniformBuffer ViewPosBuffer;

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	Mesh();
	Mesh(Renderer& renderer);
	Mesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const TextureMaterial& textureList);
	Mesh(Renderer& renderer, const TextureMaterial& textureList);
	~Mesh();

	void Draw(Renderer& renderer, int currentFrame);
	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos);
	void Destroy(Renderer& renderer);

	float* GetMeshPosPtr() { return &MeshPosition.x; };
};

