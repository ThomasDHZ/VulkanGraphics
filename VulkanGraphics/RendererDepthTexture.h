#pragma once
#include "NewTexture.h"
class RendererDepthTexture : public NewTexture
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

