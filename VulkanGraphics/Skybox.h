#pragma once
#include <vulkan\vulkan_core.h>
#include <string>
#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexBufferObject.h"
#include "Mesh.h"

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

struct SkyBoxUniformBufferObject
{
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 projection;
};

const std::vector<SkyBoxVertex> vertices =
{
	{{	-1.0f,  1.0f, -1.0f }},
	{{	-1.0f, -1.0f, -1.0f }},
	{{	 1.0f, -1.0f, -1.0f }},
	{{	 1.0f, -1.0f, -1.0f }},
	{{	 1.0f,  1.0f, -1.0f }},
	{{	-1.0f,  1.0f, -1.0f }},

	{{	-1.0f, -1.0f,  1.0f }},
	{{	-1.0f, -1.0f, -1.0f }},
	{{	-1.0f,  1.0f, -1.0f }},
	{{	-1.0f,  1.0f, -1.0f }},
	{{	-1.0f,  1.0f,  1.0f }},
	{{	-1.0f, -1.0f,  1.0f }},

	{{	 1.0f, -1.0f, -1.0f }},
	{{	 1.0f, -1.0f,  1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	 1.0f,  1.0f, -1.0f }},
	{{	 1.0f, -1.0f, -1.0f }},

	{{	-1.0f, -1.0f,  1.0f }},
	{{	-1.0f,  1.0f,  1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	 1.0f, -1.0f,  1.0f }},
	{{	-1.0f, -1.0f,  1.0f }},

	{{	-1.0f,  1.0f, -1.0f }},
	{{	 1.0f,  1.0f, -1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	 1.0f,  1.0f,  1.0f }},
	{{	-1.0f,  1.0f,  1.0f }},
	{{	-1.0f,  1.0f, -1.0f }},

	{{	-1.0f, -1.0f, -1.0f }},
	{{	-1.0f, -1.0f,  1.0f }},
	{{	 1.0f, -1.0f, -1.0f }},
	{{	 1.0f, -1.0f, -1.0f }},
	{{	-1.0f, -1.0f,  1.0f }},
	{{	 1.0f, -1.0f,  1.0f }}
};

class SkyBox
{
private:
	VulkanDevice DeviceInfo;
public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkDescriptorSetLayout DescriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	SkyBox();
	SkyBox(VulkanDevice deviceInfo);
	~SkyBox();

	void SetUpVertexBuffer(VulkanDevice deviceInfo);

	void Draw(VkCommandBuffer commandbuffer, VkDescriptorSet descriptorset, VkPipeline pipeline, VkPipelineLayout pipelineLayout);
	void DestorySwapChain(VkDevice device, int SwapChainSize);
	void Destory(VkDevice device, int SwapChainSize);
};

