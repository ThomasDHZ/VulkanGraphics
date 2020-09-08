#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
private:
    void CreateTextureView(VulkanRenderer& renderer, VkFormat format);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    Texture2D();
    Texture2D(VulkanRenderer& renderer, VkFormat format, std::string TextureLocation, unsigned int textureID);
    ~Texture2D();
};