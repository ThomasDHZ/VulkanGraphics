#pragma once
#include "Texture.h"

class RendererColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanRenderer& renderer);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    RendererColorTexture();
    RendererColorTexture(VulkanRenderer& renderer);
    ~RendererColorTexture();
};

