#include "Mesh2.h"

Mesh2::Mesh2()
{}

Mesh2::Mesh2(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, Texture tex, VkDescriptorSetLayout& descriptorSetLayout)
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

    VkDescriptorImageInfo DiffuseMap = {};
    DiffuseMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DiffuseMap.imageView = texture.View;
    DiffuseMap.sampler = texture.Sampler;

    VkDescriptorImageInfo SpecularMap = {};
    SpecularMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    SpecularMap.imageView = texture.View;
    SpecularMap.sampler = texture.Sampler;

    VkDescriptorImageInfo NormalMap = {};
    NormalMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    NormalMap.imageView = texture.View;
    NormalMap.sampler = texture.Sampler;

    VkDescriptorImageInfo DisplacementMap = {};
    DisplacementMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    DisplacementMap.imageView = texture.View;
    DisplacementMap.sampler = texture.Sampler;

    VkDescriptorImageInfo AlphaMap = {};
    AlphaMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    AlphaMap.imageView = texture.View;
    AlphaMap.sampler = texture.Sampler;

    VkDescriptorImageInfo EmissionMap = {};
    EmissionMap.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    EmissionMap.imageView = texture.View;
    EmissionMap.sampler = texture.Sampler;

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = uniformBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(UniformBufferObject);

        std::vector<WriteDescriptorSetInfo> DescriptorList;

        WriteDescriptorSetInfo PositionDescriptor;
        PositionDescriptor.DstBinding = 0;
        PositionDescriptor.DstSet = DescriptorSets[i];
        PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        PositionDescriptor.DescriptorBufferInfo = PositionInfo;
        DescriptorList.emplace_back(PositionDescriptor);

        WriteDescriptorSetInfo DiffuseMapDescriptor;
        DiffuseMapDescriptor.DstBinding = 1;
        DiffuseMapDescriptor.DstSet = DescriptorSets[i];
        DiffuseMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        DiffuseMapDescriptor.DescriptorImageInfo = DiffuseMap;
        DescriptorList.emplace_back(DiffuseMapDescriptor);

        //WriteDescriptorSetInfo SpecularMapDescriptor;
        //SpecularMapDescriptor.DstBinding = 2;
        //SpecularMapDescriptor.DstSet = DescriptorSets[i];
        //SpecularMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //SpecularMapDescriptor.DescriptorImageInfo = SpecularMap;
        //DescriptorList.emplace_back(SpecularMapDescriptor);

        //WriteDescriptorSetInfo NormalMapDescriptor;
        //NormalMapDescriptor.DstBinding = 3;
        //NormalMapDescriptor.DstSet = DescriptorSets[i];
        //NormalMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //NormalMapDescriptor.DescriptorImageInfo = NormalMap;
        //DescriptorList.emplace_back(NormalMapDescriptor);

        //WriteDescriptorSetInfo DisplacementMapDescriptor;
        //DisplacementMapDescriptor.DstBinding = 4;
        //DisplacementMapDescriptor.DstSet = DescriptorSets[i];
        //DisplacementMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //DisplacementMapDescriptor.DescriptorImageInfo = DisplacementMap;
        //DescriptorList.emplace_back(DisplacementMapDescriptor);

        //WriteDescriptorSetInfo AlphaMapDescriptor;
        //AlphaMapDescriptor.DstBinding = 5;
        //AlphaMapDescriptor.DstSet = DescriptorSets[i];
        //AlphaMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //AlphaMapDescriptor.DescriptorImageInfo = AlphaMap;
        //DescriptorList.emplace_back(AlphaMapDescriptor);

        //WriteDescriptorSetInfo EmissionMapDescriptor;
        //EmissionMapDescriptor.DstBinding = 6;
        //EmissionMapDescriptor.DstSet = DescriptorSets[i];
        //EmissionMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        //EmissionMapDescriptor.DescriptorImageInfo = EmissionMap;
        //DescriptorList.emplace_back(EmissionMapDescriptor);

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