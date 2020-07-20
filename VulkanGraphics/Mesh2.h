#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class Mesh2 : public BaseMesh
{
private:
    VulkanUniformBuffer uniformBuffer;

    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);

public:

    Mesh2();
    Mesh2(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture tex, VkDescriptorSetLayout& descriptorSetLayout);
    ~Mesh2();


    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo);
    void Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);
    void Destory(VulkanRenderer& renderer);
};

