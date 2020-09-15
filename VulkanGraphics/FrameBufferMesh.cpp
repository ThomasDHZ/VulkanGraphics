#include "FrameBufferMesh.h"

FrameBufferMesh::FrameBufferMesh() : Mesh()
{}

FrameBufferMesh::FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage) : Mesh(renderer, FrameBufferVertices, FrameBufferIndices)
{
    DiffuseMapID = FrameBufferImage;

    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateDrawMessage(renderer, 0, renderer.frameBufferRenderer.frameBufferPipeline);
}

FrameBufferMesh::~FrameBufferMesh()
{
}

void FrameBufferMesh::CreateUniformBuffers(RendererManager& renderer)
{
    frameBufferSettings = VulkanUniformBuffer(renderer, sizeof(FrameBufferSettings));
}

void FrameBufferMesh::CreateDescriptorPool(RendererManager& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};
    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    NewBaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void FrameBufferMesh::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager)
{
    NewBaseMesh::CreateDescriptorSets(renderer, renderer.frameBufferRenderer.frameBufferPipeline->ShaderPipelineDescriptorLayout);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = DiffuseMapID->GetTextureView();
    imageInfo.sampler = DiffuseMapID->GetTextureSampler();

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

        NewBaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void FrameBufferMesh::Update(RendererManager& renderer)
{
    frameBufferSettings.UpdateUniformBuffer(renderer, static_cast<void*>(&settings));
}

void FrameBufferMesh::Destory(RendererManager& renderer)
{
    frameBufferSettings.Destroy(renderer);
}