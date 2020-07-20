#pragma once
#include "Texture.h"

class RendererDepthTexture : public Texture
{
private:
    void CreateTextureImage(VulkanRenderer& renderer);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    RendererDepthTexture();
    RendererDepthTexture(VulkanRenderer& renderer);
    ~RendererDepthTexture();
};

