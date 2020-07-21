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
}

void FrameBufferMesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 1>  DescriptorPoolInfo = {};
    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
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

        WriteDescriptorSetInfo DiffuseMapDescriptor;
        DiffuseMapDescriptor.DstBinding = 0;
        DiffuseMapDescriptor.DstSet = DescriptorSets[i];
        DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DiffuseMapDescriptor.DescriptorImageInfo = imageInfo;
        DescriptorList.emplace_back(DiffuseMapDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void FrameBufferMesh::UpdateUniformBuffer(VulkanRenderer& renderer)
{
}

void FrameBufferMesh::Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

void FrameBufferMesh::Destory(VulkanRenderer& renderer)
{
    BaseMesh::Destory(renderer);
}