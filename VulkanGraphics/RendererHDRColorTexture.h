#pragma once
#include "Texture.h"

class RendererHDRColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanRenderer& renderer);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    RendererHDRColorTexture();
    RendererHDRColorTexture(VulkanRenderer& renderer);
    ~RendererHDRColorTexture();

    void RecreateRendererTexture(VulkanRenderer& renderer);
};

