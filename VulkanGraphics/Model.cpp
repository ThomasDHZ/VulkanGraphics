#include "Model.h"

Model::Model()
{
}

Model::Model(VulkanRenderer& Renderer, const std::vector<SubMesh>& SubMeshList)
{
	for (auto mesh : SubMeshList)
	{
		MeshList.emplace_back(Mesh(Renderer, mesh.VertexList, mesh.IndexList, mesh.TextureList));
	}
}

Model::~Model()
{

}

void Model::UpdateUniformBuffer(VulkanRenderer& Renderer, PositionMatrix ubo2, MeshProp viewpos)
{
	for (auto mesh : MeshList)
	{
		mesh.UpdateUniformBuffer(Renderer, ubo2, viewpos);
	}
}

void Model::Draw(VulkanRenderer& Renderer, int currentImage)
{
	for (auto mesh : MeshList)
	{
		mesh.Draw(Renderer, currentImage);
	}
}

void Model::Destroy(VulkanRenderer& Renderer)
{
	for (auto mesh : MeshList)
	{
		mesh.Destory(Renderer);
	}
}
