#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VulkanBufferManager.h"
#include "BaseShader.h"
#include "Mesh.h"
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

struct Light
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Color;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
	alignas(4) float Radius;
};

struct LightingStruct
{
	Light light;
	alignas(16) glm::vec3 viewPos;
	alignas(16) float shininess;
};

struct DebugStruct
{
	alignas(4) int DebugLayer;
};

struct UniformBufferObject2
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;

	static std::vector<DescriptorSetLayoutBindingInfo> GetDescriptorSetLayoutBindingInfo()
	{
		std::array<DescriptorSetLayoutBindingInfo, 3> LayoutBindingInfo = {};

		LayoutBindingInfo[0].Binding = 0;
		LayoutBindingInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		LayoutBindingInfo[0].StageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		LayoutBindingInfo[1].Binding = 1;
		LayoutBindingInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		LayoutBindingInfo[1].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		LayoutBindingInfo[2].Binding = 2;
		LayoutBindingInfo[2].DescriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		LayoutBindingInfo[2].StageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		return std::vector<DescriptorSetLayoutBindingInfo>(LayoutBindingInfo.begin(), LayoutBindingInfo.end());
	}
};

class Model : public Mesh
{
private:

	void CreateVertexBuffer(std::vector<Vertex> vertices);
	void CreateIndiceBuffer(std::vector<uint16_t> indices);

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkDescriptorSetLayout layout, ShaderTextureInputs TextureInfo);

public:
	Model();
	Model(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, ShaderTextureInputs shaderInput, std::vector<Vertex> vertices, std::vector<uint16_t> indices, VkDescriptorSetLayout layout);
	~Model();

	void UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage);

	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, VkDescriptorSetLayout layout, ShaderTextureInputs shaderInput);
	void Destroy();
};

