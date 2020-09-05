#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <map>
#include "RendererColorTexture.h"
#include "RendererManager.h"

class Mesh2D : public BaseMesh
{
private:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;

    void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateUniformBuffers(RendererManager& renderer);
    void CreateDescriptorPool(RendererManager& renderer);
    void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager);
    void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, Texture& texture);
    void CreateMaterialProperties();
    void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer);

public:

    std::string MeshName;
    MeshProperties properites;

    std::vector<Vertex> Vertexdata;
    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);
    float RotationAmount = 0.0f;
    bool reflect = 0;

    Mesh2D();
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int renderBit);
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, MeshTextures textures, int renderBit, Texture& texture);
    ~Mesh2D();

    void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer);
    void Destory(RendererManager& renderer);
};