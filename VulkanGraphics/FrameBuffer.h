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


	VkDeviceMemory vertexBufferMemory;

	void CreateVertexBuffer();
	void CreateDescriptorPool();
	void CreateDescriptorSets(VkImageView ColorImageView, VkImageView DepthImageView, VkDescriptorSetLayout descriptorSetLayout);
public:
	FrameBuffer();
	FrameBuffer(VulkanDevice deviceInfo, VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment, InputAttachment DepthAttachment, VkDescriptorSetLayout descriptorSetLayout);
	~FrameBuffer();

	VkBuffer vertexBuffer;
	std::vector<VkDescriptorSet> descriptorSets;

	VulkanDevice DeviceInfo;

	void Draw(FrameBufferRenderingPipeline FrameBufferPipeline, VkCommandBuffer commandbuffer, int currentImage);
	void RecreateSwapChainStage(VkExtent2D swapChainExtent, VkRenderPass renderPass, InputAttachment ColorAttachment, InputAttachment DepthAttachment, VkDescriptorSetLayout layout);
	void Destory(VkDevice device);
};

