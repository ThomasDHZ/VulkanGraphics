#pragma once
#include "NewBaseMesh.h"


struct Empty
{
    alignas(4) float empty;
};

struct CustomBuffer
{
    unsigned int ByteSize;
    VulkanUniformBuffer customBuffer;
};

class NewMesh : public NewBaseMesh
{
protected:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;
    CustomBuffer ExtendedMesProperitesBuffer;

    void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateUniformBuffers(RendererManager& renderer);
    virtual void CreateDescriptorPool(RendererManager& renderer);
    virtual void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager);
    void CreateMaterialProperties();
    void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);

public:

    std::string MeshName;
    MeshProperties properites;

    std::vector<Vertex> Vertexdata;
    float RotationAmount = 0.0f;

    NewMesh();
    NewMesh(RendererManager& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata);
    NewMesh(RendererManager& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer);
    NewMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures);
    NewMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer);
    ~NewMesh();

    void Update(RendererManager& renderer, Camera& camera, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);
    void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);
    void Destory(RendererManager& renderer);
};