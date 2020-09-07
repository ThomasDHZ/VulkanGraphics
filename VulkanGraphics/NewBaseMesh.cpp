#include "NewBaseMesh.h"
#include "VulkanBufferManager.h"
#include "RendererBase.h"

NewBaseMesh::NewBaseMesh()
{
}
NewBaseMesh::NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata, const std::vector<uint16_t>& Indicesdata)
{
    MeshVertex = VertexBuffer(renderer, Vertexdata);
    MeshIndices = IndicesBuffer(renderer, Indicesdata);
}
NewBaseMesh::NewBaseMesh(RendererManager& renderer, const std::vector<Vertex>& Vertexdata)
{
    MeshVertex = VertexBuffer(renderer, Vertexdata);
}

NewBaseMesh::~NewBaseMesh()
{
}

void NewBaseMesh::LoadTextures(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, MeshTextures textures)
{
    if (!textures.DiffuseMap.empty())
    {
        DiffuseMapID = textureManager->LoadTexture(renderer, textures.DiffuseMap, VK_FORMAT_R8G8B8A8_SRGB);
    }
    if (!textures.SpecularMap.empty())
    {
        SpecularMapID = textureManager->LoadTexture(renderer, textures.SpecularMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (!textures.NormalMap.empty())
    {
        NormalMapID = textureManager->LoadTexture(renderer, textures.NormalMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (!textures.DepthMap.empty())
    {
        DepthMapID = textureManager->LoadTexture(renderer, textures.DepthMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (!textures.AlphaMap.empty())
    {
        AlphaMapID = textureManager->LoadTexture(renderer, textures.AlphaMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (!textures.EmissionMap.empty())
    {
        EmissionMapID = textureManager->LoadTexture(renderer, textures.EmissionMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
    if (!textures.ReflectionMap.empty())
    {
        ReflectionMapID = textureManager->LoadTexture(renderer, textures.ReflectionMap, VK_FORMAT_R8G8B8A8_UNORM);
    }
}

void NewBaseMesh::CreateDescriptorPool(RendererManager& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo)
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

void NewBaseMesh::CreateDescriptorSets(RendererManager& renderer, VkDescriptorSetLayout layout)
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

void NewBaseMesh::CreateDescriptorSetsData(RendererManager& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList)
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

void NewBaseMesh::CreateDrawMessage(RendererManager& renderer, unsigned int RendererID, std::shared_ptr<GraphicsPipeline> pipeline)
{
    RendererDrawMessage DrawMessage = {};
    DrawMessage.RendererID = RendererID;
    DrawMessage.MeshVertex = MeshVertex;
    DrawMessage.MeshIndices = MeshIndices;
    DrawMessage.DescriptorSets = DescriptorSets;
    DrawMessage.pipeline = pipeline;
    
    std::shared_ptr<RendererDrawMessage> DrawMessagePtr = std::make_shared<RendererDrawMessage>(DrawMessage);
    renderer.DrawMessageList.emplace_back(DrawMessagePtr);
    DrawMessageList.emplace_back(DrawMessagePtr);
}

void NewBaseMesh::Destory(RendererManager& renderer)
{
    MeshVertex.Destory(renderer);
    MeshVertex.Destory(renderer);
    MeshIndices.Destory(renderer);

    vkDestroyDescriptorPool(renderer.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    for (auto drawMessage : DrawMessageList)
    {
        renderer.RemoveDrawMessage(drawMessage);
    }
}