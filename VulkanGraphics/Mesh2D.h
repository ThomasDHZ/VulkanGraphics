#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <map>

class Mesh2D : public BaseMesh
{
private:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;

    void LoadTiles(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout);
    void CreateMaterialProperties();
    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer);

public:

    std::string MeshName;
    MeshProperties properites;

    std::vector<Vertex> Vertexdata;
    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);
    float RotationAmount = 0.0f;

    Mesh2D();
    Mesh2D(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
    ~Mesh2D();

    void Update(VulkanRenderer& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer);
    void Destory(VulkanRenderer& renderer);
};