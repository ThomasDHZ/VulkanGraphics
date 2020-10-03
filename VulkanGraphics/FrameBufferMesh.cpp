#include "FrameBufferMesh.h"

FrameBufferMesh::FrameBufferMesh() : Mesh()
{}

FrameBufferMesh::FrameBufferMesh(RendererManager & renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage) : Mesh(renderer, FrameBufferVertices, FrameBufferIndices)
{
    DiffuseTexture = FrameBufferImage;
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateDrawMessage(renderer, 0, renderer.frameBufferRenderer.frameBufferPipeline);
}

FrameBufferMesh::FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage) : Mesh(renderer, FrameBufferVertices, FrameBufferIndices)
{
    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateDrawMessage(renderer, 0, renderer.frameBufferRenderer.frameBufferPipeline);
}

FrameBufferMesh::FrameBufferMesh(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader) : Mesh(renderer, FrameBufferVertices, FrameBufferIndices)
{
    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;
    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);
    CreateDrawMessage(renderer, effectRenderer, shader);
}

FrameBufferMesh::~FrameBufferMesh()
{
}

void FrameBufferMesh::CreateUniformBuffers(RendererManager& renderer)
{
    frameBufferSettings = VulkanUniformBuffer(renderer, sizeof(FrameBufferSettings));
}

void FrameBufferMesh::CreateDescriptorPool(RendererManager& renderer) {

    std::array<DescriptorPoolSizeInfo, 3>  DescriptorPoolInfo = {};
    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void FrameBufferMesh::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager)
{
    BaseMesh::CreateDescriptorSets(renderer, renderer.frameBufferRenderer.frameBufferPipeline->ShaderPipelineDescriptorLayout);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = DiffuseTexture->GetTextureView();
    imageInfo.sampler = DiffuseTexture->GetTextureSampler();

    VkDescriptorImageInfo BloomInfo{};
    BloomInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    BloomInfo.imageView = EmissionTexture->GetTextureView();
    BloomInfo.sampler = EmissionTexture->GetTextureSampler();

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

        WriteDescriptorSetInfo BloomMapDescriptor;
        BloomMapDescriptor.DstBinding = 1;
        BloomMapDescriptor.DstSet = DescriptorSets[i];
        BloomMapDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        BloomMapDescriptor.DescriptorImageInfo = BloomInfo;
        DescriptorList.emplace_back(BloomMapDescriptor);

        WriteDescriptorSetInfo FrameBufferSettingsDescriptor;
        FrameBufferSettingsDescriptor.DstBinding = 2;
        FrameBufferSettingsDescriptor.DstSet = DescriptorSets[i];
        FrameBufferSettingsDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        FrameBufferSettingsDescriptor.DescriptorBufferInfo = frameBufferSettingsInfo;
        DescriptorList.emplace_back(FrameBufferSettingsDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void FrameBufferMesh::Update(RendererManager& renderer)
{
    frameBufferSettings.UpdateUniformBuffer(renderer, static_cast<void*>(&settings));
}

void FrameBufferMesh::ScreenResizeUpdate(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage)
{
    vkDestroyDescriptorPool(renderer.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;

    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);

    for (auto drawMessage : DrawMessageList)
    {
        renderer.RemoveDrawMessage(drawMessage);
    }
    CreateDrawMessage(renderer, 0, renderer.frameBufferRenderer.frameBufferPipeline);
}

void FrameBufferMesh::ScreenResizeUpdate(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<Texture> FrameBufferImage, std::shared_ptr<Texture> BloomImage, int effectRenderer, std::shared_ptr<GraphicsPipeline> shader)
{
    vkDestroyDescriptorPool(renderer.Device, DescriptorPool, nullptr);
    DescriptorPool = VK_NULL_HANDLE;

    DiffuseTexture = FrameBufferImage;
    EmissionTexture = BloomImage;

    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);

    for (auto drawMessage : DrawMessageList)
    {
        renderer.RemoveDrawMessage(drawMessage);
    }
    CreateDrawMessage(renderer, effectRenderer, shader);
}

void FrameBufferMesh::Destory(RendererManager& renderer)
{
    frameBufferSettings.Destroy(renderer);
    BaseMesh::Destory(renderer);
}