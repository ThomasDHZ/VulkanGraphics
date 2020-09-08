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
    std::vector<std::shared_ptr<RendererDrawMessage>> DrawMessageList;

protected:

    VertexBuffer MeshVertex;
    IndicesBuffer MeshIndices;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    std::shared_ptr<Texture> DiffuseMapID;
    std::shared_ptr<Texture> SpecularMapID;
    std::shared_ptr<Texture> NormalMapID;
    std::shared_ptr<Texture> DepthMapID;
    std::shared_ptr<Texture> AlphaMapID;
    std::shared_ptr<Texture> EmissionMapID;
    std::shared_ptr<Texture> ReflectionMapID;

    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);
    float RotationAmount = 0.0f;

    bool MeshDeletedFlag = false;

    void LoadTextures(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateDescriptorPool(RendererManager& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
    void CreateDescriptorSets(RendererManager& renderer, VkDescriptorSetLayout layout);
    void CreateDescriptorSetsData(RendererManager& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:

    std::string MeshName;
    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    NewBaseMesh();
    NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata);
    NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata);
    ~NewBaseMesh();

    void CreateDrawMessage(RendererManager& renderer, unsigned int RendererID, std::shared_ptr<GraphicsPipeline> pipeline);
    virtual void Destory(RendererManager& renderer);

    void SetPosition2D(glm::vec2 Pos);
    void SetPosition2D(float x, float y);
    void SetPosition3D(glm::vec3 Pos);
    void SetPosition3D(float x, float y, float z);

    glm::vec2 GetPosition2D() { return glm::vec2(MeshPosition.x, MeshPosition.y); }
    glm::vec3 GetPosition3D() { return MeshPosition; }
    bool GetMeshDeletedFlag() { return MeshDeletedFlag; }
};