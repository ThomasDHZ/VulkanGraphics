#include "SkyBoxMesh.h"


SkyBoxMesh::SkyBoxMesh()
{
}

SkyBoxMesh::SkyBoxMesh(VulkanRenderer& renderer, NewCubeMapTexture texture, VkDescriptorSetLayout& descriptorSetLayout)
{
    texturebox = texture;

    VertexSize = SkyBoxVertices.size();
    IndexSize = 0;

    CreateVertexBuffer(renderer, SkyBoxVertices);
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

SkyBoxMesh::~SkyBoxMesh()
{
}

void SkyBoxMesh::CreateUniformBuffers(VulkanRenderer& renderer)
{
    PositionMatrixBuffer = VulkanUniformBuffer(renderer, sizeof(VulkanUniformBuffer));
}

void SkyBoxMesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};
    
    	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void SkyBoxMesh::CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = texture.View;
    imageInfo.sampler = texture.Sampler;

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(VulkanUniformBuffer);

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

void SkyBoxMesh::UpdateUniformBuffer(VulkanRenderer& renderer, Camera& camera)
{
    SkyBoxPositionMatrix positionMatrix = {};
    positionMatrix.view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    positionMatrix.projection = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 100.0f);
    positionMatrix.projection[1][1] *= -1;

    PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
}

void SkyBoxMesh::Update(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

void SkyBoxMesh::Destory(VulkanRenderer& renderer)
{
    PositionMatrixBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}