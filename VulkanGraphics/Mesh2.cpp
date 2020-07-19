#include "Mesh2.h"

Mesh2::Mesh2()
{}

Mesh2::Mesh2(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture2 tex, VkDescriptorSetLayout& descriptorSetLayout)
{
    texture = tex;

    VertexSize = vertexdata.size();
    IndexSize = indicesdata.size();

    CreateVertexBuffer(renderer, vertexdata);
    CreateIndexBuffer(renderer, indicesdata);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

Mesh2::~Mesh2()
{
}

void Mesh2::CreateUniformBuffers(VulkanRenderer& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
}

void Mesh2::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 7>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[3].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[4].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[5].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    DescriptorPoolInfo[6].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void Mesh2::CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

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

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void Mesh2::UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
}

void Mesh2::Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

void Mesh2::Destory(VulkanRenderer& renderer)
{
    uniformBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}