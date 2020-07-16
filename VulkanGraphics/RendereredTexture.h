#pragma once
#include "NewTexture.h"
class RendereredTexture : public NewTexture
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

