#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanBufferManager.h"
#include "BaseShader.h"
#include "Shader.h"
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

struct UniformBufferObject2
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct FragmentUniformBufferObject
{
	alignas(16) glm::vec3 cameraPos;
};


class Mesh
{
private:
	VulkanDevice DeviceInfo;
	Shader shader;

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
	Mesh(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, ShaderTextureInputs shaderInput, std::vector<Vertex> vertices, std::vector<uint16_t> indices);
	~Mesh();

	void UpdateUniformBuffer(UniformBufferObject2 ubo2, FragmentUniformBufferObject ubo3, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, ShaderTextureInputs shaderInput);
	void Destroy();
	void DestroySwapChainStage();

	Mesh& operator=(const Mesh& rhs);
};

