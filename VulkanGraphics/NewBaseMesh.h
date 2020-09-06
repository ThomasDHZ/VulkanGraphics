#pragma once
#include <vector>
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "Structs.h"
#include "GraphicsPipeline.h"
#include "BaseMesh.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"

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

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    int RenderBitFlags;

    NewBaseMesh();
    NewBaseMesh(int renderBitFlags);
    ~NewBaseMesh();

    virtual void DrawMessage();
    virtual void Destory(VulkanRenderer& renderer);
};

struct DrawMeshMessage
{
    unsigned int RendererID;
    std::shared_ptr<NewBaseMesh> ObjectMesh;
    GraphicsPipeline pipeline;
};