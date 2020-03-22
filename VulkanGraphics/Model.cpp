#include "Model.h"

Model::Model()
{
}

Model::Model(VulkanDevice deviceInfo, const std::vector<SubMesh>& SubMeshList)
{
	DeviceInfo = deviceInfo;

	for (auto mesh : SubMeshList)
	{
		MeshList.emplace_back(Mesh(deviceInfo, mesh.VertexList, mesh.IndexList, mesh.TextureList));
	}
}

Model::~Model()
{

}

void Model::UpdateUniformBuffer(UniformBufferObject ubo2, int currentImage)
{
	for (auto mesh : MeshList)
	{
		mesh.UpdateUniformBuffer(ubo2, currentImage);
	}
}


void Model::RecreateSwapChainStage()
{
	for (auto mesh : MeshList)
	{
		mesh.RecreateSwapChainStage();
	}
}

void Model::Destory()
{
	for (auto mesh : MeshList)
	{
		mesh.Destory();
	}
}
