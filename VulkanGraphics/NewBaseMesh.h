#pragma once
#include <vector>
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "Structs.h"
#include "GraphicsPipeline.h"
#include "BaseMesh.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"

struct RendererDrawMessage
{
    unsigned int RendererID;
    VertexBuffer MeshVertex;
    IndicesBuffer MeshIndices;
    std::vector<VkDescriptorSet> DescriptorSets;
    GraphicsPipeline pipeline;
};

class NewBaseMesh
{
private:

protected:

    void LoadTextures(VulkanRenderer& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateDescriptorPool(VulkanRenderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout layout);
    void CreateDescriptorSetsData(VulkanRenderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

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

    std::vector<std::shared_ptr<RendererDrawMessage>> DrawMessageList;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);

    NewBaseMesh();
    NewBaseMesh(VulkanRenderer& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata);
    NewBaseMesh(VulkanRenderer& renderer, const std::vector<Vertex>& Vertexdata);
    ~NewBaseMesh();

    std::shared_ptr<RendererDrawMessage> CreateDrawMessage(unsigned int RendererID, GraphicsPipeline pipeline);
    virtual void Destory(VulkanRenderer& renderer);
};