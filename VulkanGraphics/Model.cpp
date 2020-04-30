//#include "Model.h"
//
//Model::Model()
//{
//}
//
//Model::Model(VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList)
//{
//	DeviceInfo = deviceInfo;
//
//	for (auto mesh : SubMeshList)
//	{
//		MeshList.emplace_back(Mesh(deviceInfo, mesh.VertexList, mesh.IndexList, mesh.TextureList));
//	}
//}
//
//Model::~Model()
//{
//
//}
//
//void Model::UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.UpdateUniformBuffer(ubo2, currentImage);
//	}
//}
//
//void Model::Draw(VkCommandBuffer commandbuffer, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentImage)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.Draw(commandbuffer, ShaderPipeline, ShaderPipelineLayout, currentImage);
//	}
//}
//
//void Model::SecBufferDraw(VkCommandBuffer& commandbuffer, VkCommandBufferBeginInfo cmdInfo, VkPipeline ShaderPipeline, VkPipelineLayout ShaderPipelineLayout, int currentFrame)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.SecBufferDraw(commandbuffer, cmdInfo, ShaderPipeline, ShaderPipelineLayout, currentFrame);
//	}
//}
//
//void Model::UpdateSwapChain()
//{
//	for (int x = 0; x < MeshList.size(); x++)
//	{
//		MeshList[x].UpdateSwapChain();
//	}
//}
//
//void Model::Destory()
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.Destory();
//	}
//}
