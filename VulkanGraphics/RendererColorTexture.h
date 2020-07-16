#pragma once
#include "NewTexture.h"
class RendererColorTexture : public NewTexture
{
private:
    void CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    void CreateTextureView(VkDevice Device);
    void CreateTextureSampler(VkDevice Device);
public:
    RendererColorTexture();
    RendererColorTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    ~RendererColorTexture();
};

