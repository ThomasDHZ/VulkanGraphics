#pragma once
#include "BaseMesh.h"
#include "Mesh.h"
#include <map>
#include "RendererColorTexture.h"
#include "RendererManager.h"
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

class Mesh2D : public NewBaseMesh
{
private:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;
    CustomBuffer ExtendedMesProperitesBuffer;

    void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateUniformBuffers(RendererManager& renderer);
    void CreateDescriptorPool(RendererManager& renderer);
    void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager);
    void CreateMaterialProperties();
    void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer);
    void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo);

public:

    std::string MeshName;
    MeshProperties properites;

    std::vector<Vertex> Vertexdata;
    float RotationAmount = 0.0f;

    Mesh2D();
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures);
    Mesh2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer);
    ~Mesh2D();

    void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer);
    void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer, void* CustomBufferinfo);
    void Destory(RendererManager& renderer);
};