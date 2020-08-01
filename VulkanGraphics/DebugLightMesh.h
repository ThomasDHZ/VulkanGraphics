#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "BaseMesh.h"
#include "Camera.h"
#include "Mesh.h"

struct MeshColor
{
    alignas(16) glm::vec3 Color;
};

class DebugLightMesh : public BaseMesh
{
private:
    VulkanUniformBuffer uniformBuffer;
    VulkanUniformBuffer meshColorBuffer;

    void CreateUniformBuffers(VulkanRenderer& renderer);
    void CreateDescriptorPool(VulkanRenderer& renderer);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout);
    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, MeshColor meshColorBuffer);

public:

    glm::vec3 MeshPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 MeshRotate = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 MeshScale = glm::vec3(1.0f);
    float RotationAmount = 0.0f;

    DebugLightMesh();
    DebugLightMesh(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
    ~DebugLightMesh();

    void Update(VulkanRenderer& renderer, Camera& camera, MeshColor meshColorBuffer);
    void Destory(VulkanRenderer& renderer);
};

