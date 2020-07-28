#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"
#include "Camera.h"

//struct Material
//{
//	alignas(16) glm::vec3 Diffuse;
//	alignas(16) glm::vec3 Specular;
//	alignas(4)  float Shininess;
//	alignas(4)  float Alpha;
//	alignas(4)  float reflection;
//};
//
//struct MeshProperties
//{
//	TextureFlags MapFlags;
//	Material material;
//	alignas(8) glm::vec2 SpriteUV;
//	alignas(4) float Height;
//	alignas(4) int FlipTexture;
//};
//
//struct Lights
//{
//	DirectionalLightBuffer directionalLight;
//	PointLightBuffer pointLight;
//	SpotLightBuffer spotLight;
//	alignas(16) glm::vec3 viewPos;
//};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class Mesh : public BaseMesh
{
private:
    VulkanUniformBuffer uniformBuffer;

    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);
    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo);

public:

    std::string MeshName;

	//MeshProperties properites;

	glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	float RotationAmount = 0.0f;

    Mesh();
    Mesh(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture tex, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
    ~Mesh();

    void Update(VulkanRenderer& renderer, Camera& camera);
    void Destory(VulkanRenderer& renderer);
};

