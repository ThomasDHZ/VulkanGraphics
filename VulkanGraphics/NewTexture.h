#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>

class NewTexture
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

    NewTexture();
    NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation);
    NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec2 TextureSize);
    NewTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, glm::ivec3 TextureSize);
    ~NewTexture();
};