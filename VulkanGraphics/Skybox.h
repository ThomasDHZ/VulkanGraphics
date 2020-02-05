#pragma once
#include <vulkan\vulkan_core.h>
#include <string>
#include <array>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexBufferObject.h"
#include "UniformBufferObject.h"

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

	void SetUpCubeMapImage(VkDevice Device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void TransitionCubeMapImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void CopyBufferToCubeMapImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
public:
	VertexBufferObject<SkyBoxVertex> VBO;
	UniformBufferObject<SkyBoxUniformBufferObject> UBO;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VkDescriptorSetLayout DescriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	SkyBox();
	SkyBox(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, int SwapChainSize);
	~SkyBox();

	void SetUpDescriptorSetLayout(VkDevice Device);
	void SetUpCubeMapImage(VkDevice Device, VkCommandPool commandPool, VkQueue queue, VkPhysicalDevice physicalDevice);
	void SetUpSkyBoxImageView(VkDevice Device);
	void SetUpCubeMapSampler(VkDevice Device);
	void SetUpVertexBuffer(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice, std::vector<SkyBoxVertex> VertexData, VkCommandPool& CommandPool, VkQueue& GraphicsQueue);
	void SetUpUniformBuffers(int SwapChainSize, VkDevice device, VkPhysicalDevice physicalDevice);
	void SetUpDescriptorPool(VkDevice device, int SwapChainSize);
	void SetUpDescriptorSets(VkDevice device, int SwapChainSize);

	void UpdateUniformBuffer(SkyBoxUniformBufferObject ubo, uint32_t currentImage);
	void Draw(VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout, VkCommandBuffer commandBuffer, size_t currentImage);
	void DestorySwapChain(VkDevice device, int SwapChainSize);
	void Destory(VkDevice device, int SwapChainSize);
};

