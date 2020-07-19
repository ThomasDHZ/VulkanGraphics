#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "Vertex.h"
#include "VulkanBufferManager.h"
#include "Texture2.h"
#include "NewVulkanBufferManager.h"
#include "VulkanUniformBuffer.h"
#include "Structs.h"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class BaseMesh2
{
protected:

public:

    VkBuffer VertexBuffer;
    VkDeviceMemory VertexBufferMemory;

    VkBuffer IndexBuffer;
    VkDeviceMemory IndexBufferMemory;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    Texture2 texture;
    int IndexSize;
    int VertexSize;

    BaseMesh2()
    {
    }

    ~BaseMesh2()
    {
    }

    void createVertexBuffer(VulkanRenderer& renderer, std::vector<Vertex> vertexdata) {
        VkDeviceSize bufferSize = sizeof(vertexdata[0]) * vertexdata.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        NewVulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(renderer.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertexdata.data(), (size_t)bufferSize);
        vkUnmapMemory(renderer.Device, stagingBufferMemory);

        NewVulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

        NewVulkanBufferManager::CopyBuffer(renderer, stagingBuffer, VertexBuffer, bufferSize);

        vkDestroyBuffer(renderer.Device, stagingBuffer, nullptr);
        vkFreeMemory(renderer.Device, stagingBufferMemory, nullptr);
    }

    void createIndexBuffer(VulkanRenderer& renderer, std::vector<uint16_t> indicesdata) {
        VkDeviceSize bufferSize = sizeof(indicesdata[0]) * indicesdata.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        NewVulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(renderer.Device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indicesdata.data(), (size_t)bufferSize);
        vkUnmapMemory(renderer.Device, stagingBufferMemory);

        NewVulkanBufferManager::CreateBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);

        NewVulkanBufferManager::CopyBuffer(renderer, stagingBuffer, IndexBuffer, bufferSize);

        vkDestroyBuffer(renderer.Device, stagingBuffer, nullptr);
        vkFreeMemory(renderer.Device, stagingBufferMemory, nullptr);
    }

    void CreateDescriptorPool(VulkanRenderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
    {
        std::vector<VkDescriptorPoolSize> DescriptorPoolList = {};

        for (auto DescriptorPool : DescriptorPoolInfo)
        {
            VkDescriptorPoolSize DescriptorPoolBinding = {};
            DescriptorPoolBinding.type = DescriptorPool.DescriptorType;
            DescriptorPoolBinding.descriptorCount = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());
            DescriptorPoolList.emplace_back(DescriptorPoolBinding);
        }

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(DescriptorPoolList.size());
        poolInfo.pPoolSizes = DescriptorPoolList.data();
        poolInfo.maxSets = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());

        if (vkCreateDescriptorPool(renderer.Device, &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout layout)
    {
        std::vector<VkDescriptorSetLayout> layouts(renderer.SwapChain.GetSwapChainImageCount(), layout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(renderer.SwapChain.GetSwapChainImageCount());
        allocInfo.pSetLayouts = layouts.data();

        DescriptorSets.resize(renderer.SwapChain.GetSwapChainImageCount());
        if (vkAllocateDescriptorSets(renderer.Device, &allocInfo, DescriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }
    }

    void CreateDescriptorSetsData(VulkanRenderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
    {
        std::vector<VkWriteDescriptorSet>  WriteDescriptorInfo = {};

        for (int x = 0; x < descriptorWritesList.size(); x++)
        {
            VkWriteDescriptorSet DescriptorSet = {};
            DescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            DescriptorSet.dstSet = descriptorWritesList[x].DstSet;
            DescriptorSet.dstBinding = descriptorWritesList[x].DstBinding;
            DescriptorSet.dstArrayElement = 0;
            DescriptorSet.descriptorType = descriptorWritesList[x].DescriptorType;
            DescriptorSet.descriptorCount = 1;
            if (descriptorWritesList[x].DescriptorType == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            {
                DescriptorSet.pBufferInfo = &descriptorWritesList[x].DescriptorBufferInfo;
            }
            else
            {
                DescriptorSet.pImageInfo = &descriptorWritesList[x].DescriptorImageInfo;
            }
            WriteDescriptorInfo.emplace_back(DescriptorSet);
        }

        vkUpdateDescriptorSets(renderer.Device, static_cast<uint32_t>(WriteDescriptorInfo.size()), WriteDescriptorInfo.data(), 0, nullptr);
    }

    void Destory(VulkanRenderer& renderer)
    {
        if (VertexSize != 0)
        {
            vkDestroyDescriptorPool(renderer.Device, DescriptorPool, nullptr);
            vkDestroyBuffer(renderer.Device, VertexBuffer, nullptr);
            vkFreeMemory(renderer.Device, VertexBufferMemory, nullptr);

            DescriptorPool = VK_NULL_HANDLE;
            VertexBuffer = VK_NULL_HANDLE;
            VertexBufferMemory = VK_NULL_HANDLE;
        }

        if (IndexSize != 0)
        {
            vkDestroyBuffer(renderer.Device, IndexBuffer, nullptr);
            vkFreeMemory(renderer.Device, IndexBufferMemory, nullptr);

            IndexBuffer = VK_NULL_HANDLE;
            IndexBufferMemory = VK_NULL_HANDLE;
        }
    }
};

class Mesh2 : public BaseMesh2
{
private:
    VulkanUniformBuffer uniformBuffer;
public:

    Mesh2()
    {}

    Mesh2(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture2 tex, VkDescriptorSetLayout& descriptorSetLayout)
    {
        texture = tex;

        VertexSize = vertexdata.size();
        IndexSize = indicesdata.size();

        createVertexBuffer(renderer, vertexdata);
        createIndexBuffer(renderer, indicesdata);
        createUniformBuffers(renderer);
        createDescriptorPool(renderer);
        createDescriptorSets(renderer, descriptorSetLayout);
    }

    void createUniformBuffers(VulkanRenderer& renderer)
    {
        uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    }

    void createDescriptorPool(VulkanRenderer& renderer) {

        std::array<DescriptorPoolSizeInfo, 7>  DescriptorPoolInfo = {};

        DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;


        BaseMesh2::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
    }

    void createDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
    {
        BaseMesh2::CreateDescriptorSets(renderer, descriptorSetLayout);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = texture.View;
        imageInfo.sampler = texture.Sampler;

        for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer.GetUniformBuffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            std::vector<WriteDescriptorSetInfo> DescriptorList;

            WriteDescriptorSetInfo PositionDescriptor;
            PositionDescriptor.DstBinding = 0;
            PositionDescriptor.DstSet = DescriptorSets[i];
            PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            PositionDescriptor.DescriptorBufferInfo = bufferInfo;
            DescriptorList.emplace_back(PositionDescriptor);

            WriteDescriptorSetInfo DiffuseMapDescriptor;
            DiffuseMapDescriptor.DstBinding = 1;
            DiffuseMapDescriptor.DstSet = DescriptorSets[i];
            DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            DiffuseMapDescriptor.DescriptorImageInfo = imageInfo;
            DescriptorList.emplace_back(DiffuseMapDescriptor);

            BaseMesh2::CreateDescriptorSetsData(renderer, DescriptorList);
        }
    }

    void UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo)
    {
        uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
    }

    void Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
    {
        createUniformBuffers(renderer);
        createDescriptorPool(renderer);
        createDescriptorSets(renderer, descriptorSetLayout);
    }

    void Destory(VulkanRenderer& renderer)
    {
        uniformBuffer.Destroy(renderer);
        BaseMesh2::Destory(renderer);
    }
};

