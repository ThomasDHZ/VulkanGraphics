#pragma once
#include <vulkan\vulkan_core.h>
#include <string>
#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CubeMapTexture.h"
#include "SkyBoxPipeline.h"

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
	void SetUpDescriptorPool(VulkanRenderer& Renderer);
	void SetUpDescriptorSets(VulkanRenderer& Renderer);

public:
	CubeMapTexture CubeMap;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	SkyBox();
	SkyBox(VulkanRenderer& Renderer, CubeMapTexture texture);
	~SkyBox();

	void UpdateUniformBuffer(VulkanRenderer& Renderer, SkyBoxUniformBufferObject ubo, uint32_t currentImage);
	void Draw(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, VkCommandBuffer commandBuffer, size_t currentImage);
};


