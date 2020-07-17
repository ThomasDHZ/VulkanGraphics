#pragma once
#include "Texture2.h"
class RendereredTexture : public Texture2
{
private:
    void CreateTextureImage(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    void CreateTextureView(VkDevice Device);
    void CreateTextureSampler(VkDevice Device);
public:
    RendereredTexture();
    RendereredTexture(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkExtent2D extent);
    ~RendereredTexture();
};

