#pragma once
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <array>

#include "UniformBufferObject.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CubeTexture.h"

struct CubeMapVertex
{
	glm::vec3 pos;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription BindingDescription = {};
		BindingDescription.binding = 0;
		BindingDescription.stride = sizeof(CubeMapVertex);
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
		AttributeDescription.offset = offsetof(CubeMapVertex, pos);
		AttributeDescriptions.emplace_back(AttributeDescription);

		return AttributeDescriptions;
	}
};

const std::vector<CubeMapVertex> CubeVertices =
{
		 {{-0.5f, -0.5f, -0.5f}},
		 {{0.5f, -0.5f, -0.5f}},
		 {{0.5f,  0.5f, -0.5f}},
		 {{0.5f,  0.5f, -0.5f}},
		{{-0.5f,  0.5f, -0.5f}},
		{{-0.5f, -0.5f, -0.5f}},

		{{-0.5f, -0.5f,  0.5f}},
		 {{0.5f, -0.5f,  0.5f}},
		 {{0.5f,  0.5f,  0.5f}},
		 {{0.5f,  0.5f,  0.5f}},
		{{-0.5f,  0.5f,  0.5f}},
		{{-0.5f, -0.5f,  0.5f}},

		{{-0.5f,  0.5f,  0.5f}},
		{{-0.5f,  0.5f, -0.5f}},
		{{-0.5f, -0.5f, -0.5f}},
		{{-0.5f, -0.5f, -0.5f}},
		{{-0.5f, -0.5f,  0.5f}},
		{{-0.5f,  0.5f,  0.5f}},

		 {{0.5f,  0.5f,  0.5f}},
		 {{0.5f,  0.5f, -0.5f}},
		 {{0.5f, -0.5f, -0.5f}},
		 {{0.5f, -0.5f, -0.5f}},
		 {{0.5f, -0.5f,  0.5f}},
		 {{0.5f,  0.5f,  0.5f}},

		{{-0.5f, -0.5f, -0.5f}},
		{{0.5f, -0.5f, -0.5f}},
		 {{0.5f, -0.5f,  0.5f}},
		 {{0.5f, -0.5f,  0.5f}},
		{{-0.5f, -0.5f,  0.5f}},
		{{-0.5f, -0.5f, -0.5f}},

		{{-0.5f,  0.5f, -0.5f}},
		{{0.5f,  0.5f, -0.5f}},
		 {{0.5f,  0.5f,  0.5f}},
		 {{0.5f,  0.5f,  0.5f}},
		{{-0.5f,  0.5f,  0.5f}},
		{{-0.5f,  0.5f, -0.5f}}
};


struct SkyBoxUniformBufferObject2 
{
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

class CubeMapMesh
{
private:
	VertexBufferObject<CubeMapVertex> VBO;
	CubeTexture texture;

	UniformBufferObject<SkyBoxUniformBufferObject2> UniformBufferobject;

	VkDescriptorPool DescriptorPool;
	std::vector<VkDescriptorSet> DescriptorSets;
public:
	CubeMapMesh();
	CubeMapMesh(std::vector<std::string> TexturePath, int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<VkCommandBuffer> CommandBuffer, std::vector<CubeMapVertex> VertexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue, VkDescriptorSetLayout DescriptorSetLayout);
	~CubeMapMesh();

	void SetUpDescriptorPool(int SwapChainSize, VkDevice device);
	void SetUpDescriptorSets(int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout);
	void UpdateUniformBuffers(SkyBoxUniformBufferObject2 ubo, uint32_t ImageIndex);
	void UpdateTexture(CubeTexture updateTexture, int SwapChainSize, VkDevice device, VkDescriptorSetLayout DescriptorSetLayout);
	void Draw(VkCommandBuffer CommandBuffer, VkPipeline Pipeline, VkPipelineLayout PipeLineLayout, int frame);
	void Destory(VkDevice device, int FrameSize);

	VkDescriptorPool GetVkDescriptorPool() { return DescriptorPool; }
	CubeMapMesh& operator=(const CubeMapMesh& rhs);
};

