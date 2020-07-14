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
#include "Camera.h"
#include "EngineRenderedTexture.h"

struct Material
{
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
	alignas(4)  float Shininess;
	alignas(4)  float Alpha;
	alignas(4)  float reflection;
};

struct MeshProperties
{
	TextureFlags MapFlags;
	Material material;
	alignas(8) glm::vec2 SpriteUV;
	alignas(4) float Height;
	alignas(4) int FlipTexture;
};

struct Lights
{
	DirectionalLightBuffer directionalLight;
	PointLightBuffer pointLight;
	SpotLightBuffer spotLight;
	alignas(16) glm::vec3 viewPos;
};

class Mesh : public BaseMesh
{
private:
	UniformBuffer PositionMatrixBuffer;
	UniformBuffer MeshPropertiesBuffer;
	UniformBuffer LightsBuffer;

protected:
	void CreateMaterialProperties(const TextureMaps& textureList);
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);
	void CalcTangent();

public:

	std::string MeshName;

	MeshProperties properites;

	glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	float RotationAmount = 0.0f;

	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, Lights light);

	Mesh();
	Mesh(Renderer& renderer);
	Mesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const TextureMaps& textureList);
	Mesh(Renderer& renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList, VkImageView RenderedTextureImageView, VkSampler RendereredTextureImageSampler);
	Mesh(Renderer& renderer, const TextureMaps& textureList);
	~Mesh();

	void Update(Renderer& renderer, Camera& camera, Lights light);
	void UpdateGUI();
	void Draw(Renderer& renderer, int currentFrame);
	void Destroy(Renderer& renderer);
	void UpdateDescriptorSets(Renderer& renderer, EngineRenderedTexture engineTexture);

	float* GetMeshPosPtr() { return &MeshPosition.x; };
};

