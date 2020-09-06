#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"
#include "Camera.h"
#include "CubeMapTexture.h"
#include "OrthographicCamera.h"

struct DirectionalLightStruct {
    alignas(16) glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);

    alignas(16) glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    alignas(16) glm::vec3 diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    alignas(16) glm::vec3 specular = glm::vec3(0.5f, 0.5f, 0.5f);
};

struct PointLightStruct {
    alignas(16) glm::vec3 position;

    alignas(4) float constant = 1.0f;
    alignas(4) float linear = 0.09f;
    alignas(4) float quadratic = 0.032;

    alignas(16) glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    alignas(16) glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    alignas(16) glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct SpotLightStruct {
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    alignas(4) float cutOff;
    alignas(4) float outerCutOff;

    alignas(4) float constant;
    alignas(4) float linear;
    alignas(4) float quadratic;

    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

struct MapBits
{
    alignas(4) int UseDiffuseMapBit = 0;
    alignas(4) int UseSpecularMapBit = 0;
    alignas(4) int UseNormalMapBit = 0;
    alignas(4) int UseDepthMapBit = 1;
    alignas(4) int UseAlphaMapBit = 0;
    alignas(4) int UseEmissionMapBit = 0;
    alignas(4) int UseSkyBoxBit = 0;
};

struct Material 
{
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
    alignas(4) float shininess;
    alignas(4) float reflectivness;
};

struct MeshProperties
{
    Material material;
    alignas(8) glm::vec2 UVOffset = glm::vec2(0.0f, 0.0f);
    alignas(4) int UseDiffuseMapBit = 0;
    alignas(4) int UseSpecularMapBit = 0;
    alignas(4) int UseNormalMapBit = 0;
    alignas(4) int UseDepthMapBit = 0;
    alignas(4) int UseAlphaMapBit = 0;
    alignas(4) int UseEmissionMapBit = 0;
    alignas(4) int UseReflectionMapBit = 0;
    alignas(4) int UseSkyBoxBit;
    alignas(4) float minLayers;
    alignas(4) float maxLayers;
    alignas(4) float heightScale;
    alignas(4) float timer;
};

struct LightBufferObject
{
    DirectionalLightStruct dLight;
    PointLightStruct pLight;
    SpotLightStruct sLight;
    alignas(16) glm::vec3 viewPos;
};

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class Mesh : public BaseMesh
{
private:
    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;

    void LoadTextures(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, int cubemap, VkDescriptorSetLayout& descriptorSetLayout);
    void CreateMaterialProperties();
    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer);

public:

    std::string MeshName;
    MeshProperties properites;

	glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 MeshScale = glm::vec3(1.0f);
	float RotationAmount = 0.0f;

    Mesh();
    Mesh(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int cubemap, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
    ~Mesh();

    void Update(VulkanRenderer& renderer, Camera& camera, LightBufferObject Lightbuffer);
    void Update(VulkanRenderer& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer);
    void Destory(VulkanRenderer& renderer);
};

