#pragma once
#include "Texture.h"

class RendererColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RendererColorTexture();
    RendererColorTexture(VulkanEngine& renderer);
    ~RendererColorTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};

