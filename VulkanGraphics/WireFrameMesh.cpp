//#include "WireFrameMesh.h"
//
//WireFrameMesh::WireFrameMesh() : BaseMesh()
//{
//}
//
//WireFrameMesh::WireFrameMesh(Renderer& renderer, const std::vector<Vertex>& vertexList, const std::vector<uint16_t>& indexList) : BaseMesh(renderer, vertexList, indexList)
//{
//	CreateUniformBuffers(renderer);
//	CreateDescriptorPool(renderer);
//	CreateDescriptorSets(renderer);
//}
//
//WireFrameMesh::~WireFrameMesh()
//{
//}
//
//void WireFrameMesh::CreateUniformBuffers(Renderer& renderer)
//{
//	PositionMatrixBuffer = UniformBuffer(renderer, sizeof(PositionMatrix));
//}
//
//void WireFrameMesh::CreateDescriptorPool(Renderer& renderer)
//{
//	std::array<DescriptorPoolSizeInfo, 1>  DescriptorPoolInfo = {};
//
//	DescriptorPoolInfo[0].DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//
//	BaseMesh::CreateDescriptorPool(renderer, std::vector<DescriptorPoolSizeInfo>(DescriptorPoolInfo.begin(), DescriptorPoolInfo.end()));
//}
//
//void WireFrameMesh::CreateDescriptorSets(Renderer& renderer)
//{
//	//BaseMesh::CreateDescriptorSets(renderer, *GetDescriptorSetLayout(renderer));
//
//	//for (size_t i = 0; i < GetSwapChainImageCount(renderer); i++)
//	//{
//	//	VkDescriptorBufferInfo PositionInfo = {};
//	//	PositionInfo.buffer = PositionMatrixBuffer.GetUniformBuffer(i);
//	//	PositionInfo.offset = 0;
//	//	PositionInfo.range = sizeof(PositionMatrix);
//
//	//	std::vector<WriteDescriptorSetInfo> DescriptorList;
//
//	//	WriteDescriptorSetInfo PositionDescriptor;
//	//	PositionDescriptor.DstBinding = 0;
//	//	PositionDescriptor.DstSet = descriptorSets[i];
//	//	PositionDescriptor.DescriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//	//	PositionDescriptor.DescriptorBufferInfo = PositionInfo;
//	//	DescriptorList.emplace_back(PositionDescriptor);
//
//	//	BaseMesh::CreateDescriptorSetsData(renderer, DescriptorList);
//	//}
//}
//
//void WireFrameMesh::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix)
//{
//	PositionMatrixBuffer.UpdateUniformBuffer(renderer, static_cast<void*>(&positionMatrix));
//}
//
//void WireFrameMesh::Destroy(Renderer& renderer)
//{
//	PositionMatrixBuffer.Destroy(renderer);
//
//	BaseMesh::Destory(renderer);
//}