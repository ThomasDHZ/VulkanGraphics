#pragma once
#include "Texture2.h"
class RendererDepthTexture : public Texture2
{
private:
    void CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    void CreateTextureView(VkDevice Device);
    void CreateTextureSampler(VkDevice Device);
public:
    RendererDepthTexture();
    RendererDepthTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    ~RendererDepthTexture();
};

