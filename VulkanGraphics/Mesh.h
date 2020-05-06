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

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Lighter
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Color;
	alignas(16) glm::vec3 CameraPosition;
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
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	std::vector<VkBuffer>  AmbientLightUniformBuffers;
	std::vector<VkDeviceMemory>  AmbientLightUniformBuffersMemory;

	std::vector<VkBuffer>  LighterUniformBuffers;
	std::vector<VkDeviceMemory> LighterUniformBuffersMemory;

	glm::vec3 MeshPosition = glm::vec3();
	glm::vec3 MeshRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);

	Mesh();
	Mesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices, const std::vector<Texture2D>& textureList);
	Mesh(VulkanRenderer& Renderer, const std::vector<Texture2D>& textureList);
	~Mesh();

	void Draw(VulkanRenderer& Renderer, int currentFrame);
	void UpdateUniformBuffer(VulkanRenderer& Renderer, UniformBufferObject ubo2, AmbientLightUniformBuffer light, Lighter lighter, int currentImage);
	void Destroy(VulkanRenderer& Renderer);
};

