#pragma once
#include "Texture2.h"
class NewTexture2D : public Texture2
{
private:
    void LoadTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation);
    void CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice);
    void CreateTextureView(VkDevice Device);
    void CreateTextureSampler(VkDevice Device);
public:
    NewTexture2D();
    NewTexture2D(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::string TextureLocation);
    ~NewTexture2D();
};