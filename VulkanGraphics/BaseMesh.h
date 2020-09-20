#pragma once
#include <vector>
#include "Vertex.h"
#include "VulkanRenderer.h"
#include "Structs.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndicesBuffer.h"
#include "RendererBase.h"
#include "RendererManager.h"
#include "Camera.h"
#include "OrthographicCamera.h"
#include "CubeMapTexture.h"
#include "TextureManager.h"
#include "VulkanUniformBuffer.h"

const  std::string DefaultTexture = "texture/DefaultTexture.bmp";
enum RenderBitFlag
{
    RenderOnMainPass = 1 << 0,
    RenderOnTexturePass = 1 << 1,
    RenderOnFrameBufferPass = 1 << 2,
    RenderShadow = 1 << 3,
    RenderMainPass = 1 << 4,
    RenderBloomPass1 = 1 << 5
};
struct MeshTextures
{

    std::string DiffuseMap = DefaultTexture;
    std::string SpecularMap = DefaultTexture;
    std::string NormalMap = DefaultTexture;
    std::string DepthMap = DefaultTexture;
    std::string AlphaMap = DefaultTexture;
    std::string EmissionMap = DefaultTexture;
    std::string ReflectionMap = DefaultTexture;
    std::string CubeMap[6];

    std::shared_ptr<Texture> RendererDiffuseMap;
    std::shared_ptr<Texture> RendererSpecularMap;
    std::shared_ptr<Texture> RendererNormalMap;
    std::shared_ptr<Texture> RendererDepthMap;
    std::shared_ptr<Texture> RendererAlphaMap;
    std::shared_ptr<Texture> RendererEmissionMap;
    std::shared_ptr<Texture> RendererReflectionMap;
    std::shared_ptr<CubeMapTexture> RendererCubeMap;
};


class BaseMesh
{
private:

protected:

    VertexBuffer MeshVertex;
    IndicesBuffer MeshIndices;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    std::shared_ptr<Texture> DiffuseTexture;
    std::shared_ptr<Texture> SpecularTexture;

    std::shared_ptr<Texture> DepthTexture;
    std::shared_ptr<Texture> AlphaTexture;
    std::shared_ptr<Texture> EmissionTexture;
    std::shared_ptr<Texture> ReflectionTexture;
    std::shared_ptr<CubeMapTexture> SkyBoxTexture;

    std::vector<std::shared_ptr<RendererDrawMessage>> DrawMessageList;

    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);
    float RotationAmount = 0.0f;

    bool MeshDeletedFlag = false;

    void LoadTextures(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateDescriptorPool(RendererManager& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
    void CreateDescriptorSets(RendererManager& renderer, VkDescriptorSetLayout layout);
    void CreateDescriptorSetsData(RendererManager& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:
    std::shared_ptr<Texture> NormalTexture;
    std::string MeshName;
    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    BaseMesh();
    BaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata);
    BaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata);
    ~BaseMesh();

    void CreateDrawMessage(RendererManager& renderer, unsigned int RendererID, std::shared_ptr<GraphicsPipeline> pipeline);
    virtual void Update(RendererManager& renderer);
    virtual void Destory(RendererManager& renderer);

    void SetPosition2D(glm::vec2 Pos);
    void SetPosition2D(float x, float y);
    void SetPosition3D(glm::vec3 Pos);
    void SetPosition3D(float x, float y, float z);

    glm::vec2 GetPosition2D() { return glm::vec2(MeshPosition.x, MeshPosition.y); }
    glm::vec3 GetPosition3D() { return MeshPosition; }
    bool GetMeshDeletedFlag() { return MeshDeletedFlag; }
};