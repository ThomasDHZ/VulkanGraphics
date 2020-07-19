#include "Texture2.h"
#include "NewVulkanBufferManager.h"
#include "VulkanUniformBuffer.h"
#include "Structs.h"
#include "Vertex.h"

class BaseMesh
{
private:
protected:
    void CreateVertexBuffer(VulkanRenderer& renderer, std::vector<Vertex> vertexdata);
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

    Texture2 texture;
    int IndexSize;
    int VertexSize;

    BaseMesh();
    ~BaseMesh();

    void Destory(VulkanRenderer& renderer);
};