#pragma once
#include <vulkan\vulkan_core.h>
#include <string>
#include <array>

#include "Structs.h"
#include "BaseMesh.h"
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

const std::vector<SkyBoxVertex> SkyboxVertices =
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

	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateDescriptorSets(SkyBoxPipeline pipeline, CubeMapTexture cubeMapTexture);
	void CreateVertexBuffer(VulkanDevice deviceInfo);

public:
	SkyBox();
	SkyBox(VulkanDevice deviceInfo, SkyBoxPipeline pipeline, CubeMapTexture cubeMapTexture);
	~SkyBox();

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	void Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage);
	void Destory();
};

