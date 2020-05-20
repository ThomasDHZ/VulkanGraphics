#include "DebugLightMesh.h"

DebugLightMesh::DebugLightMesh() : BaseMesh()
{
}

DebugLightMesh::DebugLightMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) : BaseMesh(Renderer, vertices, indices)
{
	CreateUniformBuffers(Renderer);
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer);
}

DebugLightMesh::DebugLightMesh(VulkanRenderer& Renderer, const std::vector<Vertex>& vertices) : BaseMesh(Renderer, vertices)
{
	CreateUniformBuffers(Renderer);
	CreateDescriptorPool(Renderer);
	CreateDescriptorSets(Renderer);
}

DebugLightMesh::~DebugLightMesh()
{
}

void DebugLightMesh::CreateUniformBuffers(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer = UniformBuffer(Renderer, sizeof(PositionMatrix));
	LighterBuffers = UniformBuffer(Renderer, sizeof(DirectionalLightBuffer));
}

void DebugLightMesh::CreateDescriptorPool(VulkanRenderer& Renderer)
{
	std::array<DescriptorPoolSizeInfo, 2>  DescriptorPoolInfo = {};

	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	DescriptorPoolInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

	BaseMesh::CreateDescriptorPool(Renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
}

void DebugLightMesh::CreateDescriptorSets(VulkanRenderer& Renderer)
{
	const auto pipeline = GetDebugLightPipeline(Renderer);
	BaseMesh::CreateDescriptorSets(Renderer, pipeline.ShaderPipelineDescriptorLayout);

	for (size_t i = 0; i < GetSwapChainImageCount(Renderer); i++)
	{
		VkDescriptorBufferInfo PositionInfo = {};
		PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
		PositionInfo.offset = 0;
		PositionInfo.range = sizeof(PositionMatrix);

		VkDescriptorBufferInfo LightInfo = {};
		LightInfo.buffer = LighterBuffers.GetUniformBuffer(i);
		LightInfo.offset = 0;
		LightInfo.range = sizeof(DirectionalLightBuffer);

		std::array<WriteDescriptorSetInfo, 2>  WriteDescriptorInfo = {};

		WriteDescriptorInfo[0].DstBinding = 0;
		WriteDescriptorInfo[0].DstSet = descriptorSets[i];
		WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[0].DescriptorBufferInfo = PositionInfo;

		WriteDescriptorInfo[1].DstBinding = 1;
		WriteDescriptorInfo[1].DstSet = descriptorSets[i];
		WriteDescriptorInfo[1].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		WriteDescriptorInfo[1].DescriptorBufferInfo = LightInfo;

		CreateDescriptorSetsData(Renderer, std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
	}
}

void DebugLightMesh::Draw(VulkanRenderer& Renderer, int currentFrame)
{
	const auto pipeline = GetDebugLightPipeline(Renderer);
	VkBuffer vertexBuffers[] = { vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	if (Renderer.Settings.ShowMeshLines)
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
	}
	else
	{
		vkCmdBindPipeline(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
	}

	vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(Renderer, currentFrame), 0, 1, vertexBuffers, offsets);
	vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(Renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
	if (IndiceSize == 0)
	{
		vkCmdDraw(*GetSecondaryCommandBuffer(Renderer, currentFrame), VertexSize, 1, 0, 0);
	}
	else
	{
		vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(Renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(*GetSecondaryCommandBuffer(Renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
	}
}

void DebugLightMesh::UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix positionMatrix, DirectionalLightBuffer lighter)
{
	PositionMatrixBuffer.UpdateUniformBuffer(Renderer, static_cast<void*>(&positionMatrix), Renderer.DrawFrame);
	LighterBuffers.UpdateUniformBuffer(Renderer, static_cast<void*>(&lighter), Renderer.DrawFrame);
}

void DebugLightMesh::Destroy(VulkanRenderer& Renderer)
{
	PositionMatrixBuffer.Destroy(Renderer);
	LighterBuffers.Destroy(Renderer);

	BaseMesh::Destory(Renderer);
}
