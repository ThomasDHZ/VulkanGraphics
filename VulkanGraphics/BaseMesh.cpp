#include "BaseMesh.h"

BaseMesh::BaseMesh()
{
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::CreateVertexBuffer(VulkanRenderer& renderer, std::vector<Vertex> vertexdata) {
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

void BaseMesh::CreateIndexBuffer(VulkanRenderer& renderer, std::vector<uint16_t> indicesdata) {
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

void BaseMesh::CreateDescriptorPool(VulkanRenderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
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

void BaseMesh::CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout layout)
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

void BaseMesh::CreateDescriptorSetsData(VulkanRenderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
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

void BaseMesh::Destory(VulkanRenderer& renderer)
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