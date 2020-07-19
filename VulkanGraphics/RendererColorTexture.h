#pragma once
#include "Texture2.h"
class RendererColorTexture : public Texture2
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

