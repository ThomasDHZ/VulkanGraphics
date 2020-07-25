#pragma once
#include "Texture.h"

class RendereredTexture : public Texture
{
private:
    void CreateTextureImage(VulkanRenderer& renderer);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    RendereredTexture();
    RendereredTexture(VulkanRenderer& renderer);
    ~RendereredTexture();

    void RecreateRendererTexture(VulkanRenderer& renderer);
};

