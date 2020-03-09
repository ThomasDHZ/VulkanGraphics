#include "Model.h"

Model::Model()
{
}

Model::Model(MainPipeline pipeline, VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList)
{
	DeviceInfo = deviceInfo;

	for (auto mesh : SubMeshList)
	{
		MeshList.emplace_back(Mesh(pipeline, deviceInfo, mesh.VertexList, mesh.IndexList, mesh.TextureList));
	}
}

Model::~Model()
{

}

void Model::UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage)
{
	for (auto mesh : MeshList)
	{
		mesh.UpdateUniformBuffer(ubo2, currentImage);
	}
}

void Model::Draw(VkCommandBuffer commandbuffer, MainPipeline pipeline, int currentImage)
{
	for (auto mesh : MeshList)
	{
		mesh.Draw(commandbuffer, pipeline, currentImage);
	}
}

void Model::RecreateSwapChainStage(MainPipeline pipeline)
{
	for (auto mesh : MeshList)
	{
		mesh.RecreateSwapChainStage(pipeline);
	}
}

void Model::Destory()
{
	for (auto mesh : MeshList)
	{
		mesh.Destory();
	}
}
