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
private:

protected:

    VkDeviceMemory Memory = VK_NULL_HANDLE;
    VkImage Image = VK_NULL_HANDLE;
    VkSampler Sampler = VK_NULL_HANDLE;

    void TransitionImageLayout(VulkanRenderer& renderer, VkImageLayout oldLayout, VkImageLayout newLayout);
    void CopyBufferToImage(VulkanRenderer& renderer, VkBuffer buffer);

    virtual void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation, VkFormat format);
    virtual void CreateTextureImage(VulkanRenderer& renderer, VkImageCreateInfo TextureInfo);
    virtual void CreateTextureView(VulkanRenderer& renderer, VkImageViewCreateInfo TextureImageViewInfo);
    virtual void CreateTextureSampler(VulkanRenderer& renderer, VkSamplerCreateInfo TextureImageSamplerInfo);

public:
    VkImageView View = VK_NULL_HANDLE;
    VkDescriptorSet ImGuiDescriptorSet = VK_NULL_HANDLE;

    unsigned int TextureID = 0;
    std::string FileName;
    TextureType TypeOfTexture;

    int Width;
    int Height;

    Texture();
    Texture(VulkanRenderer& renderer, std::string TextureLocation, unsigned int textureID, TextureType textureType, VkFormat format);
    Texture(VulkanRenderer& renderer, std::string TextureLocation, TextureType textureType, VkFormat format);
    Texture(VulkanRenderer& renderer, unsigned int textureID, TextureType textureType);
    Texture(VulkanRenderer& renderer, TextureType textureType);
    ~Texture();

    virtual void Delete(VulkanRenderer& renderer);

    VkImageView GetTextureView() { return View; }
    VkSampler GetTextureSampler() { return Sampler; }
};