#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>
#include "VulkanRenderer.h"
#include "VulkanBufferManager.h"

enum TextureType
{
    Color,
    Depth
};

class Texture
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

    Texture();
    Texture(VulkanRenderer& renderer, std::string TextureLocation, TextureType textureType);
    Texture(VulkanRenderer& renderer, TextureType textureType);
    ~Texture();

    void Delete(VulkanRenderer& renderer);
};