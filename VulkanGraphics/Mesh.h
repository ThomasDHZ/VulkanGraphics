#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include "Structs.h"
#include "BaseShader.h"

class Mesh
{
private:

protected:
	int VerticeSize;
	int IndiceSize;

	VulkanDevice DeviceInfo;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	void CreateDescriptorPool(std::vector<DescriptorPoolSizeInfo> DescriptorPoolInfo);
	void CreateDescriptorSets(VkDescriptorSetLayout layout);
	void CreateDescriptorSetsData(std::vector<WriteDescriptorSetInfo> descriptorWritesList);

public:
	Mesh();
	Mesh(VulkanDevice deviceInfo);
	~Mesh();

	void UpdateUniformBuffer(VkDeviceMemory UniformBufferMemory, void* UniformObjectData, VkDeviceSize UniformSize);
	void Draw(VkCommandBuffer commandbuffer, VkPipeline pipeline, VkPipelineLayout layout, int currentImage);

	void Destory();
	void DestorySwapChain();
};

