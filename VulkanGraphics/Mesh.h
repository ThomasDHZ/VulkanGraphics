#pragma once
#include "NewBaseMesh.h"

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
    alignas(4) int ReflectSprite;
    alignas(8) glm::vec2 UVScale = glm::vec2(1.0f, 1.0f);
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

struct Empty
{
    alignas(4) float empty;
};

struct CustomBuffer
{
    unsigned int ByteSize;
    VulkanUniformBuffer customBuffer;
};

class Mesh : public NewBaseMesh
{
protected:

    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer lightBuffer;
    VulkanUniformBuffer meshPropertiesBuffer;
    CustomBuffer ExtendedMesProperitesBuffer;

    void LoadTiles(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures);
    void CreateMaterialProperties();

    virtual void CreateUniformBuffers(RendererManager& renderer);
    virtual void CreateDescriptorPool(RendererManager& renderer);
    virtual void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager);
    virtual void UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);

public:

    std::string MeshName;
    MeshProperties properites;

    std::vector<Vertex> Vertexdata;
    float RotationAmount = 0.0f;

    Mesh();
    Mesh(RendererManager& renderer, const std::vector<Vertex>& vertexdata);
    Mesh(RendererManager& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata);
    Mesh(RendererManager& renderer, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, CustomBuffer customBuffer);
    Mesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures);
    Mesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, const std::vector<Vertex>& vertexdata, const std::vector<uint16_t>& indicesdata, MeshTextures textures, CustomBuffer customBuffer);
    ~Mesh();

    virtual void Update(RendererManager& renderer) override;
    virtual void Update(RendererManager& renderer, Camera& camera, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);
    virtual void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject Lightbuffer, void* CustomBufferinfo = nullptr);
    virtual void Destory(RendererManager& renderer) override;
};