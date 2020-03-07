#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VulkanBufferManager.h"
#include "BaseMesh.h"
#include "Texture2D.h"
#include "MainPipeline.h"


struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexureCoord;
	glm::vec3 Tangant;
	glm::vec3 BiTangant;

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
		AttributeDescription.offset = offsetof(Vertex, Position);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 1;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, Normal);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 2;
		AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, TexureCoord);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 3;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, Tangant);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 4;
		AttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex, BiTangant);
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

class Mesh : public BaseMesh
{
private:
	std::vector<Vertex> VertexList;
	std::vector<unsigned int> IndexList;

	void CreateVertexBuffer();
	void CreateIndiceBuffer();

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(MainPipeline pipeline);
public:
	std::vector<Texture2D> TextureList;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	Mesh();
	Mesh(MainPipeline pipeline, VulkanDevice deviceInfo, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2D> textureList);
	~Mesh();

	void UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage);

	void RecreateSwapChainStage(MainPipeline pipeline);
};

