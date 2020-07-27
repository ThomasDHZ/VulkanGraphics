#pragma once
#include "Texture.h"
#include "VulkanBufferManager.h"
#include "VulkanUniformBuffer.h"
#include "Structs.h"
#include "Vertex.h"

enum RendererBitFlag
{
    Nonw = 0,
    RenderOnMainPass =  1 << 0,
    RenderOnTexturePass = 1 << 1,
    RenderOnFrameBufferPass = 1 << 2
};

class BaseMesh
{
private:
protected:
    void CreateVertexBuffer(VulkanRenderer& renderer, std::vector<Vertex> vertexdata);
    void CreateVertexBuffer(VulkanRenderer& renderer, std::vector<Vertex2D> vertexdata);
    void CreateIndexBuffer(VulkanRenderer& renderer, std::vector<uint16_t> indicesdata);
    void CreateDescriptorPool(VulkanRenderer& renderer, std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
    void CreateDescriptorSets(VulkanRenderer& renderer, VkDescriptorSetLayout layout);
    void CreateDescriptorSetsData(VulkanRenderer& renderer, std::vector<WriteDescriptorSetInfo> descriptorWritesList);
public:

    VkBuffer VertexBuffer;
    VkDeviceMemory VertexBufferMemory;

    VkBuffer IndexBuffer;
    VkDeviceMemory IndexBufferMemory;

    VkDescriptorPool DescriptorPool;
    std::vector<VkDescriptorSet> DescriptorSets;

    Texture texture;
    int IndexSize;
    int VertexSize;
    int RenderBitFlags;

    BaseMesh();
    BaseMesh(int renderBitFlags);
    ~BaseMesh();

    void Destory(VulkanRenderer& renderer);
};