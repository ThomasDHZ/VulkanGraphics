#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
private:
    void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    Texture2D();
    Texture2D(VulkanRenderer& renderer, std::string TextureLocation);
    ~Texture2D();
};