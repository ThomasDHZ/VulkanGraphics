#pragma once
#include <vulkan\vulkan_core.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex2D
{
	glm::vec2 Position;
	glm::vec2 TexureCoord;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};
		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(Vertex2D);
		BindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return BindingDescription;
	}

	static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> AttributeDescriptions = {};

		VkVertexInputAttributeDescription AttributeDescription;

		AttributeDescription.binding = 0;
		AttributeDescription.location = 0;
		AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex2D, Position);
		AttributeDescriptions.emplace_back(AttributeDescription);

		AttributeDescription.binding = 0;
		AttributeDescription.location = 1;
		AttributeDescription.format = VK_FORMAT_R32G32_SFLOAT;
		AttributeDescription.offset = offsetof(Vertex2D, TexureCoord);
		AttributeDescriptions.emplace_back(AttributeDescription);

		return AttributeDescriptions;
	}
};

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

struct SkyBoxVertex
{
	glm::vec3 pos;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(SkyBoxVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 1> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(SkyBoxVertex, pos);

		return attributeDescriptions;
	}
};
