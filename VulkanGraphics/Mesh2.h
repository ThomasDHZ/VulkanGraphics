#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <iostream>
#include <fstream>
#include "NewTexture.h"
#include "Vertex.h"
#include "VulkanBufferManager.h"

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

struct Mesh2
{

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkCommandPool commandPool;
    VkQueue graphicsQueue;
    std::vector<VkImage> swapChainImages;

    VkBuffer VertexBuffer;
    VkDeviceMemory VertexBufferMemory;

    VkBuffer IndexBuffer;
    VkDeviceMemory IndexBufferMemory;

    std::vector<VkBuffer> UniformBuffers;
    std::vector<VkDeviceMemory> UniformBuffersMemory;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    NewTexture texture;

    Mesh2()
    {}

    Mesh2(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, std::vector<VkImage> SwapChainImages, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, NewTexture tex, VkDescriptorSetLayout& descriptorSetLayout)
    {
        texture = tex;
        device = Device;
        physicalDevice = PhysicalDevice;
        commandPool = CommandPool;
        graphicsQueue = GraphicsQueue;
        swapChainImages = SwapChainImages;

        createVertexBuffer(vertexdata);
        createIndexBuffer(indicesdata);
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets(descriptorSetLayout);
    }

    void createVertexBuffer(std::vector<Vertex> vertexdata) {
        VkDeviceSize bufferSize = sizeof(vertexdata[0]) * vertexdata.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanBufferManager::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertexdata.data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        VulkanBufferManager::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VertexBuffer, VertexBufferMemory);

        VulkanBufferManager::CopyBuffer(device, physicalDevice, stagingBuffer, VertexBuffer, bufferSize, commandPool, graphicsQueue);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createIndexBuffer(std::vector<uint16_t> indicesdata) {
        VkDeviceSize bufferSize = sizeof(indicesdata[0]) * indicesdata.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VulkanBufferManager::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indicesdata.data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        VulkanBufferManager::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, IndexBuffer, IndexBufferMemory);

        VulkanBufferManager::CopyBuffer(device, physicalDevice, stagingBuffer, IndexBuffer, bufferSize, commandPool, graphicsQueue);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createUniformBuffers() {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);

        UniformBuffers.resize(swapChainImages.size());
        UniformBuffersMemory.resize(swapChainImages.size());

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VulkanBufferManager::CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, UniformBuffers[i], UniformBuffersMemory[i]);
        }
    }

    void createDescriptorPool() {
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &DescriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }
    }

    void createDescriptorSets(VkDescriptorSetLayout& descriptorSetLayout)
    {
        std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = DescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        DescriptorSets.resize(swapChainImages.size());
        if (vkAllocateDescriptorSets(device, &allocInfo, DescriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = UniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = texture.View;
            imageInfo.sampler = texture.Sampler;

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = DescriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = DescriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    void Draw(VkCommandBuffer commandBuffer, VkPipeline pipeline, VkPipelineLayout layout, std::vector<uint16_t>indices, int frame)
    {
        VkBuffer vertexBuffers[] = { VertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, IndexBuffer, 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &DescriptorSets[frame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
    }

    void Update(VkDescriptorSetLayout& descriptorSetLayout)
    {
        createUniformBuffers();
        createDescriptorPool();
        createDescriptorSets(descriptorSetLayout);
    }
};

