#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>

enum TextureType
{
    Color,
    Depth
};

class Texture2
{
protected:
    int Width;
    int Height;

    void TransitionImageLayout(VkDevice Device, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VkDevice Device, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkBuffer buffer);

    virtual void CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkImageCreateInfo TextureInfo);
    virtual void CreateTextureView(VkDevice Device, VkImageViewCreateInfo TextureImageViewInfo);
    virtual void CreateTextureSampler(VkDevice Device, VkSamplerCreateInfo TextureImageSamplerInfo);

public:
    VkDeviceMemory Memory;
    VkImage Image;
    VkImageView View;
    VkSampler Sampler;

    Texture2();
    Texture2(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation);
    Texture2(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec2 TextureSize);
    Texture2(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec3 TextureSize);
    ~Texture2();

    void Delete(VkDevice Device);
};