#pragma once
#include "Texture2.h"

class NewTexture2D : public Texture2
{
private:
    void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    NewTexture2D();
    NewTexture2D(VulkanRenderer& renderer, std::string TextureLocation);
    ~NewTexture2D();
};