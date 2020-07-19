#pragma once
#include "Texture2.h"

class RendererDepthTexture : public Texture2
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

