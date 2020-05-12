#pragma once
#include "FrameBufferRenderingPipeline.h"
#include "InputAttachment.h"
#include "VulkanBufferManager.h"
#include "Structs.h"
#include "Vertex.h"

const std::vector<Vertex2D> FrameBufferVertices =
{
	{{-1.0f,  1.0f}, {0.0f, 1.0f}},
	{{-1.0f, -1.0f}, {0.0f, 0.0f}},
	{{ 1.0f, -1.0f}, {1.0f, 0.0f}},

	{{-1.0f,  1.0f}, {0.0f, 1.0f}},
	{{ 1.0f, -1.0f}, {1.0f, 0.0f}},
	{{ 1.0f,  1.0f}, {1.0f, 1.0f}}
};

class FrameBuffer
{
private:

	VkDescriptorPool descriptorPool;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	void CreateVertexBuffer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue);
	void CreateDescriptorPool(VkDevice Device, int SwapChainSize);
	void CreateDescriptorSets(VkDevice Device, VkImageView ColorImageView, VkImageView DepthImageView, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize);
public:
	FrameBuffer();
	FrameBuffer(VkDevice Device, VkPhysicalDevice PhysicalDevice, VkCommandPool CommandPool, VkQueue GraphicsQueue, VkExtent2D swapChainExtent, VkRenderPass renderPass,
				InputAttachment ColorAttachment, InputAttachment DepthAttachment, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize);
	~FrameBuffer();

	std::vector<VkDescriptorSet> descriptorSets;


	void Draw(FrameBufferRenderingPipeline FrameBufferPipeline, VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkDevice Device, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment,
								InputAttachment DepthAttachment, VkDescriptorSetLayout descriptorSetLayout, int SwapChainSize);
	void Destory(VkDevice device);
};

