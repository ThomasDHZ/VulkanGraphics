#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "MainPipeline.h"

class Mesh
{
private:

protected:
	int VertexSize;
	int IndiceSize;

	VulkanDevice DeviceInfo;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:
	Mesh();
	Mesh(VulkanDevice deviceInfo);
	~Mesh();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	virtual void Draw(VkCommandBuffer commandbuffer, MainPipeline pipeline, int currentImage);

	void Destory();
	void DestorySwapChain();
};

