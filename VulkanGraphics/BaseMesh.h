#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"

class BaseMesh
{
private:

protected:
	VulkanDevice DeviceInfo;

	int VertexSize;
	uint16_t IndiceSize;

	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorPool descriptorPool;
	BaseMesh();
	BaseMesh(VulkanDevice deviceInfo);
	~BaseMesh();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);

	virtual void ClearSwapChain();
	virtual void Destory();
};

