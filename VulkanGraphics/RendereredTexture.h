#pragma once
#include "Texture2.h"
#include "VulkanRenderer.h"
class RendereredTexture : public Texture2
{
private:
    void CreateTextureImage(VulkanRenderer& renderer);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    RendereredTexture();
    RendereredTexture(VulkanRenderer& renderer);
    ~RendereredTexture();
};

