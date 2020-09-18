#include "DebugLightMesh.h"
#include "Texture2D.h"

DebugLightMesh::DebugLightMesh() : Mesh()
{
}

DebugLightMesh::DebugLightMesh(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, int renderBit) : Mesh(renderer, CubeVertices, CubeIndices)
{

    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, textureManager);

    if (renderBit & RenderBitFlag::RenderOnMainPass)
    {
       CreateDrawMessage(renderer, 1, renderer.forwardRenderer.DebugLightPipeline);
    }
    if (renderBit & RenderBitFlag::RenderOnTexturePass)
    {
        CreateDrawMessage(renderer, 2, renderer.textureRenderer.DebugLightPipeline);
    }
}

DebugLightMesh::~DebugLightMesh()
{
}

void DebugLightMesh::CreateUniformBuffers(RendererManager& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    meshColorBuffer = VulkanUniformBuffer(renderer, sizeof(MeshColor));
}

void DebugLightMesh::CreateDescriptorPool(RendererManager& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void DebugLightMesh::CreateDescriptorSets(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager)
{
    BaseMesh::CreateDescriptorSets(renderer, renderer.forwardRenderer.DebugLightPipeline->ShaderPipelineDescriptorLayout);

    for (size_t i = 0; i < renderer.SwapChain.GetSwapChainImageCount(); i++)
    {
        VkDescriptorBufferInfo PositionInfo = {};
        PositionInfo.buffer = uniformBuffer.GetUniformBuffer(i);
        PositionInfo.offset = 0;
        PositionInfo.range = sizeof(UniformBufferObject);

        VkDescriptorBufferInfo MeshColorInfo = {};
        MeshColorInfo.buffer = meshColorBuffer.GetUniformBuffer(i);
        MeshColorInfo.offset = 0;
        MeshColorInfo.range = sizeof(MeshColor);

        std::vector<WriteDescriptorSetInfo> DescriptorList;

        WriteDescriptorSetInfo PositionDescriptor;
        PositionDescriptor.DstBinding = 0;
        PositionDescriptor.DstSet = DescriptorSets[i];
        PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        PositionDescriptor.DescriptorBufferInfo = PositionInfo;
        DescriptorList.emplace_back(PositionDescriptor);

        WriteDescriptorSetInfo MeshColorDescriptor;
        MeshColorDescriptor.DstBinding = 1;
        MeshColorDescriptor.DstSet = DescriptorSets[i];
        MeshColorDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        MeshColorDescriptor.DescriptorBufferInfo = MeshColorInfo;
        DescriptorList.emplace_back(MeshColorDescriptor);

        BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
    }
}

void DebugLightMesh::Update(RendererManager& renderer, std::shared_ptr<Camera>& camera, MeshColor MeshColorBuffer)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera->GetViewMatrix();
    ubo.proj = camera->GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    UpdateUniformBuffer(renderer, ubo, MeshColorBuffer);
    Mesh::UpdateUniformBuffer(renderer, ubo);
}

void DebugLightMesh::UpdateUniformBuffer(RendererManager& renderer, UniformBufferObject ubo, MeshColor MeshColorBuffer)
{
    meshColorBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&MeshColorBuffer));
    Mesh::UpdateUniformBuffer(renderer, ubo);
}

void DebugLightMesh::Destory(RendererManager& renderer)
{
    uniformBuffer.Destroy(renderer);
    meshColorBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}