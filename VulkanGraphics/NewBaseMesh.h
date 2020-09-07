#pragma once
#include <vector>
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "Structs.h"
#include "GraphicsPipeline.h"
#include "BaseMesh.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"
#include "RendererBase.h"
#include "RendererManager.h"

class NewBaseMesh
{
private:

protected:

    void LoadTextures(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateDescriptorPool(RendererManager& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
    void CreateDescriptorSets(RendererManager& renderer, VkDescriptorSetLayout layout);
    void CreateDescriptorSetsData(RendererManager& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:

    unsigned int DiffuseMapID;
    unsigned int SpecularMapID;
    unsigned int NormalMapID;
    unsigned int DepthMapID;
    unsigned int AlphaMapID;
    unsigned int EmissionMapID;
    unsigned int ReflectionMapID;

    VertexBuffer MeshVertex;
    IndicesBuffer MeshIndices;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    std::vector<std::shared_ptr<RendererDrawMessage>> DrawMessageList;

    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);

    NewBaseMesh();
    NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata);
    NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata);
    ~NewBaseMesh();

    void CreateDrawMessage(RendererManager& renderer, unsigned int RendererID, std::shared_ptr<GraphicsPipeline> pipeline);
    virtual void Destory(RendererManager& renderer);
};