//#include "DebugLightMesh.h"
//
//DebugLightMesh::DebugLightMesh() : BaseMesh()
//{
//}
//
//DebugLightMesh::DebugLightMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) : BaseMesh(renderer, vertices, indices)
//{
//	CreateUniformBuffers(renderer);
//	CreateDescriptorPool(renderer);
//	CreateDescriptorSets(renderer);
//}
//
//DebugLightMesh::DebugLightMesh(Renderer& renderer, const std::vector<Vertex>& vertices) : BaseMesh(renderer, vertices)
//{
//	CreateUniformBuffers(renderer);
//	CreateDescriptorPool(renderer);
//	CreateDescriptorSets(renderer);
//}
//
//DebugLightMesh::~DebugLightMesh()
//{
//}
//
//void DebugLightMesh::CreateUniformBuffers(Renderer& renderer)
//{
//	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(PositionMatrix));
//}
//
//void DebugLightMesh::CreateDescriptorPool(Renderer& renderer)
//{
//	std::array<DescriptorPoolSizeInfo, 1>  DescriptorPoolInfo = {};
//
//	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//
//	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
//}
//
//void DebugLightMesh::CreateDescriptorSets(Renderer& renderer)
//{
//	//const auto pipeline = GetDebugLightPipeline(renderer);
//	//BaseMesh::CreateDescriptorSets(renderer, pipeline.ShaderPipelineDescriptorLayout);
//
//	//for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
//	//{
//	//	VkDescriptorBufferInfo PositionInfo = {};
//	//	PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
//	//	PositionInfo.offset = 0;
//	//	PositionInfo.range = sizeof(PositionMatrix);
//
//	//	std::array<WriteDescriptorSetInfo, 1>  WriteDescriptorInfo = {};
//
//	//	WriteDescriptorInfo[0].DstBinding = 0;
//	//	WriteDescriptorInfo[0].DstSet = descriptorSets[i];
//	//	WriteDescriptorInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	//	WriteDescriptorInfo[0].DescriptorBufferInfo = PositionInfo;
//
//	//	CreateDescriptorSetsData(renderer, std::vector<WriteDescriptorSetInfo>(WriteDescriptorInfo.begin(), WriteDescriptorInfo.end()));
//	//}
//}
//
//void DebugLightMesh::Draw(Renderer& renderer, int currentFrame)
//{
//	//const auto pipeline = GetDebugLightPipeline(renderer);
//	//VkBuffer vertexBuffers[] = { vertexBuffer };
//	//VkDeviceSize offsets[] = { 0 };
//
//	//if (renderer.Settings.ShowMeshLines)
//	//{
//	//	vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
//	//}
//	//else
//	//{
//	//	vkCmdBindPipeline(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipeline);
//	//}
//
//	//vkCmdBindVertexBuffers(*GetSecondaryCommandBuffer(renderer, currentFrame), 0, 1, vertexBuffers, offsets);
//	//vkCmdBindDescriptorSets(*GetSecondaryCommandBuffer(renderer, currentFrame), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.ShaderPipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
//	//if (IndiceSize == 0)
//	//{
//	//	vkCmdDraw(*GetSecondaryCommandBuffer(renderer, currentFrame), VertexSize, 1, 0, 0);
//	//}
//	//else
//	//{
//	//	vkCmdBindIndexBuffer(*GetSecondaryCommandBuffer(renderer, currentFrame), indexBuffer, 0, VK_INDEX_TYPE_UINT16);
//	//	vkCmdDrawIndexed(*GetSecondaryCommandBuffer(renderer, currentFrame), static_cast<uint32_t>(IndiceSize), 1, 0, 0, 0);
//	//}
//}
//
//void DebugLightMesh::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix)
//{
//	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
//}
//
//void DebugLightMesh::Destroy(Renderer& renderer)
//{
//	PositionMatrixBuffer.Destroy(renderer);
//
//	BaseMesh::Destory(renderer);
//}
