#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>
#include "VulkanRenderer.h"

enum TextureType
{
    Color,
    Depth
};

class Texture2
{
protected:
    enum TextureType
    {
        vkTexture2D,
        vkTextureCube
    };

    std::string FileName;
    TextureType TypeOfTexture;

    int Width;
    int Height;

    void TransitionImageLayout(VulkanRenderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VulkanRenderer& renderer, VkBuffer buffer);

    virtual void CreateTextureImage(VulkanRenderer& renderer, VkImageCreateInfo TextureInfo);
    virtual void CreateTextureView(VulkanRenderer& renderer, VkImageViewCreateInfo TextureImageViewInfo);
    virtual void CreateTextureSampler(VulkanRenderer& renderer, VkSamplerCreateInfo TextureImageSamplerInfo);

public:
    VkDeviceMemory Memory;
    VkImage Image;
    VkImageView View;
    VkSampler Sampler;

    Texture2();
    Texture2(VulkanRenderer& renderer, std::string TextureLocation, TextureType textureType);
    Texture2(VulkanRenderer& renderer, TextureType textureType);
    ~Texture2();

    void Delete(VulkanRenderer& renderer);
};