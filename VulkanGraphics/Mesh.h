#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanBufferManager.h"

struct VulkanDevice
{
	VkDevice Device;
	VkPhysicalDevice PhysicalDevice;
	VkCommandPool CommandPool;
	VkQueue GraphicsQueue;
	int SwapChainSize;
};

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};
		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(Vertex);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return BindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

		VkVertexInputAttributeDescription AttributeDescription;

		AttributeDescription.binding = 0;
		AttributeDescription.location = 0;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, pos);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 1;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, normal);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 2;
		AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, texCoord);
		AttributeDescriptions.emplace_back(AttributeDescription);

		return AttributeDescriptions;
	}
};

class Mesh
{
private:
	VulkanDevice DeviceInfo;

	int VerticeSize;
	int IndiceSize;

	void CreateVertexBuffer(std::vector<Vertex> vertices);
	void CreateIndiceBuffer(std::vector<uint16_t> indices);

public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	Mesh();
	Mesh(VulkanDevice deviceInfo, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
	~Mesh();

	void Draw(VkCommandBuffer commandbuffer, VkDescriptorSet descriptorset, VkPipeline pipeline, VkPipelineLayout pipelineLayout);
	void Destroy();

	Mesh& operator=(const Mesh& rhs);
};

