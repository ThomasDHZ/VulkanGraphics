#pragma once
#include "Texture.h"

class SpriteTexture2D : public Texture
{
private:
    void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation, VkFormat format);
    void CreateTextureView(VulkanRenderer& renderer, VkFormat format);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    SpriteTexture2D();
    SpriteTexture2D(VulkanRenderer& renderer, VkFormat format, std::string TextureLocation, unsigned int textureID);
    ~SpriteTexture2D();
};