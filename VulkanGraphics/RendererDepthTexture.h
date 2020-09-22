#pragma once
#include "Texture.h"

class RendererDepthTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RendererDepthTexture();
    RendererDepthTexture(VulkanEngine& renderer);
    ~RendererDepthTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};

