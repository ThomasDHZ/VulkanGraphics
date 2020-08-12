#pragma once
#include <vulkan/vulkan.h>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <stb_image.h>
#include "VulkanRenderer.h"
#include "VulkanBufferManager.h"
#include "ImGui/imgui_impl_vulkan.h"

enum TextureType
{
    vkTexture2D,
    vkTextureCube,
    vkRenderedTexture
};

class Texture
{
protected:
    void TransitionImageLayout(VulkanRenderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VulkanRenderer& renderer, VkBuffer buffer);

    virtual void CreateTextureImage(VulkanRenderer& renderer, VkImageCreateInfo TextureInfo);
    virtual void CreateTextureView(VulkanRenderer& renderer, VkImageViewCreateInfo TextureImageViewInfo);
    virtual void CreateTextureSampler(VulkanRenderer& renderer, VkSamplerCreateInfo TextureImageSamplerInfo);

public:
    unsigned int TextureID = 0;
    std::string FileName;
    TextureType TypeOfTexture;
    VkDeviceMemory Memory = VK_NULL_HANDLE;
    VkImage Image = VK_NULL_HANDLE;
    VkImageView View = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;
    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

    int Width;
    int Height;

    Texture();
    Texture(VulkanRenderer& renderer, std::string TextureLocation, TextureType textureType);
    Texture(VulkanRenderer& renderer, TextureType textureType);
    ~Texture();

    void Delete(VulkanRenderer& renderer);
};