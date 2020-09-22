#pragma once
#include "Texture.h"

class RendererHDRColorTexture : public Texture
{
private:
    void CreateTextureImage(VulkanEngine& renderer);
    void CreateTextureView(VulkanEngine& renderer);
    void CreateTextureSampler(VulkanEngine& renderer);
public:
    RendererHDRColorTexture();
    RendererHDRColorTexture(VulkanEngine& renderer);
    ~RendererHDRColorTexture();

    void RecreateRendererTexture(VulkanEngine& renderer);
};

