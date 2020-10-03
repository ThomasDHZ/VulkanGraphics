#pragma once
#include "Mesh.h"

const std::vector<Vertex> FrameBufferVertices =
{
    {{1.0f, 1.0f, 0.0f},   {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
    {{-1.0f, 1.0f, 0.0f},  {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
};

const std::vector<uint16_t> FrameBufferIndices = {
    0, 1, 3, 1, 2, 3
};

struct FrameBufferSettings
{
    alignas(4) float Gamma = 2.2f;
    alignas(4) float HDRValue = 1.0f;
};

class FrameBufferMesh : public Mesh
{
private:

    VulkanUniformBuffer frameBufferSettings;

    void CreateUniformBuffers(RendererManager& renderer) override;
    void CreateDescriptorPool(RendererManager& renderer) override;
    void CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager) override;

public:

    FrameBufferMesh();
    FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage);
    FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage);
    FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader);
    ~FrameBufferMesh();

    FrameBufferSettings settings;

    void Update(RendererManager& renderer);
    void ScreenResizeUpdate(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage);
    void ScreenResizeUpdate(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader);
    void Destory(RendererManager& renderer) override;
};

