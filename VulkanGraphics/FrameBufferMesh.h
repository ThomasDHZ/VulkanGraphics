#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"


const std::vector<Vertex2D> FrameBufferVertices =
{
    {{1.0f, 1.0f}, {1.0f, 1.0f}},
    {{1.0f, -1.0f}, {1.0f, 0.0f}},
    {{-1.0f, -1.0f}, {0.0f, 0.0f}},
    {{-1.0f, 1.0f}, {0.0f, 1.0f}},
};

const std::vector<uint16_t> FrameBufferIndices = {
    0, 1, 3, 1, 2, 3
};

struct FrameBufferSettings
{
    alignas(4) float Gamma = 2.2f;
    alignas(4) float HDRValue = 1.0f;
};

class FrameBufferMesh : public BaseMesh
{
private:

    VulkanUniformBuffer frameBufferSettings;

    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);

public:

    FrameBufferMesh();
    FrameBufferMesh(VulkanRenderer& renderer, Texture FrameBufferImage, VkDescriptorSetLayout& descriptorSetLayout);
    ~FrameBufferMesh();

    FrameBufferSettings settings;

    void UpdateUniformBuffer(VulkanRenderer& renderer);
    void UpdateSwapChain(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout, Texture FrameBufferImage);
    void Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);
    void Destory(VulkanRenderer& renderer);
};

