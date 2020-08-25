#include "DebugLightMesh.h"
#include "Texture2D.h"

DebugLightMesh::DebugLightMesh() : BaseMesh()
{
}

DebugLightMesh::DebugLightMesh(VulkanRenderer& renderer, std::vector<Vertex> vertexdata, std::vector<uint16_t> indicesdata, VkDescriptorSetLayout& descriptorSetLayout, int renderBit) : BaseMesh(renderBit)
{
    VertexSize = vertexdata.size();
    IndexSize = indicesdata.size();

    CreateVertexBuffer(renderer, vertexdata);

    if (IndexSize != 0)
    {
        CreateIndexBuffer(renderer, indicesdata);
    }

    CreateUniformBuffers(renderer);
    CreateDescriptorPool(renderer);
    CreateDescriptorSets(renderer, descriptorSetLayout);
}

DebugLightMesh::~DebugLightMesh()
{
}

void DebugLightMesh::CreateUniformBuffers(VulkanRenderer& renderer)
{
    uniformBuffer = VulkanUniformBuffer(renderer, sizeof(UniformBufferObject));
    meshColorBuffer = VulkanUniformBuffer(renderer, sizeof(MeshColor));
}

void DebugLightMesh::CreateDescriptorPool(VulkanRenderer& renderer) {

    std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

    DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

    BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void DebugLightMesh::CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout& descriptorSetLayout)
{
    BaseMesh::CreateDescriptorSets(renderer, descriptorSetLayout);

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

void DebugLightMesh::Update(VulkanRenderer& renderer, Camera& camera, MeshColor MeshColorBuffer)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera.GetViewMatrix();
    ubo.proj = glm::perspective(glm::radians(camera.Zoom), renderer.SwapChain.GetSwapChainResolution().width / (float)renderer.SwapChain.GetSwapChainResolution().height, 0.1f, 10000.0f);
    ubo.proj[1][1] *= -1;

    if (RotationAmount != 0 &&
        (MeshRotate.x != 0 ||
            MeshRotate.y != 0 ||
            MeshRotate.z != 0))
    {
        ubo.model = glm::rotate(ubo.model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    }

    UpdateUniformBuffer(renderer, ubo, MeshColorBuffer);
}

void DebugLightMesh::Update(VulkanRenderer& renderer, OrthographicCamera& camera, MeshColor MeshColorBuffer)
{
    UniformBufferObject ubo{};
    ubo.model = glm::mat4(1.0f);
    ubo.model = glm::translate(ubo.model, MeshPosition);
    ubo.model = glm::scale(ubo.model, MeshScale);
    ubo.view = camera.GetViewMatrix();
    ubo.proj = camera.GetProjectionMatrix();
    ubo.proj[1][1] *= -1;

    UpdateUniformBuffer(renderer, ubo, MeshColorBuffer);
}

void DebugLightMesh::UpdateUniformBuffer(VulkanRenderer& renderer, UniformBufferObject ubo, MeshColor MeshColorBuffer)
{
    uniformBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&ubo));
    meshColorBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&MeshColorBuffer));
}

void DebugLightMesh::Destory(VulkanRenderer& renderer)
{
    uniformBuffer.Destroy(renderer);
    meshColorBuffer.Destroy(renderer);
    BaseMesh::Destory(renderer);
}