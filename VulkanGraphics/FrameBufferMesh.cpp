#include "FrameBufferMesh.h"

FrameBufferMesh::FrameBufferMesh()
{}

FrameBufferMesh::FrameBufferMesh(VulkanRenderer& renderer, Texture tex, VkDescriptorSetLayout& descriptorSetLayout)
{
    texture = tex;

    VertexSize = FrameBufferVertices.size();
    IndexSize = FrameBufferIndices.size();

    CreateVertexBuffer(renderer, FrameBufferVertices);
    CreateIndexBuffer(renderer, FrameBufferIndices);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

FrameBufferMesh::~FrameBufferMesh()
{
}

void FrameBufferMesh::CreateUniformBuffers(VulkanRenderer& renderer)
{
    frameBufferSettings = VulkanUniformBuffer(renderer, sizeof(FrameBufferSettings));
}

void FrameBufferMesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};
    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void FrameBufferMesh::CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture.View;
    imageInfo.sampler = texture.Sampler;

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        std::vector<WriteDescriptorSetInfo> DescriptorList;

        VkDescriptorBufferInfo frameBufferSettingsInfo{};
        frameBufferSettingsInfo.buffer = frameBufferSettings.GetUniformBuffer(i);
        frameBufferSettingsInfo.offset = 0;
        frameBufferSettingsInfo.range = sizeof(FrameBufferSettings);

        WriteDescriptorSetInfo DiffuseMapDescriptor;
        DiffuseMapDescriptor.DstBinding = 0;
        DiffuseMapDescriptor.DstSet = DescriptorSets[i];
        DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DiffuseMapDescriptor.DescriptorImageInfo = imageInfo;
        DescriptorList.emplace_back(DiffuseMapDescriptor);

        WriteDescriptorSetInfo FrameBufferSettingsDescriptor;
        FrameBufferSettingsDescriptor.DstBinding = 1;
        FrameBufferSettingsDescriptor.DstSet = DescriptorSets[i];
        FrameBufferSettingsDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        FrameBufferSettingsDescriptor.DescriptorBufferInfo = frameBufferSettingsInfo;
        DescriptorList.emplace_back(FrameBufferSettingsDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void FrameBufferMesh::UpdateUniformBuffer(VulkanRenderer& renderer)
{
    frameBufferSettings.UpdateUniformBuffer(renderer, static_cast<void*>(&settings));
}

void FrameBufferMesh::Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

void FrameBufferMesh::Destory(VulkanRenderer& renderer)
{
    frameBufferSettings.Destroy(renderer);
    BaseMesh::Destory(renderer);
}