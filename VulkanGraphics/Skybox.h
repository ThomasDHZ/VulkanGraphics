#pragma once
#include <vulkan\vulkan_core.h>
#include <string>
#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CubeMapTexture.h"
#include "SkyBoxPipeline.h"

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

class SkyBox : public BaseMesh
{
private:
	void SetUpVertexBuffer();
	void SetUpUniformBuffers();
	void SetUpDescriptorPool();
	void SetUpDescriptorSets(SkyBoxPipeline pipeline);

public:
	CubeMapTexture CubeMap;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	SkyBox();
	SkyBox(VulkanDevice vulkanDevice, CubeMapTexture texture, SkyBoxPipeline pipeline);
	~SkyBox();

	void UpdateUniformBuffer(SkyBoxUniformBufferObject ubo, uint32_t currentImage);
	void Draw(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, VkCommandBuffer commandBuffer, size_t currentImage);
	void UpdateSwapChain(SkyBoxPipeline pipeline);
	void DestorySwapChain();
};


