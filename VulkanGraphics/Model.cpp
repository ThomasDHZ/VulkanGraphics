//#include "Model.h"
//
//Model::Model()
//{
//}
//
//Model::Model(Renderer& renderer, const std::vector<SubMesh>& SubMeshList)
//{
//	for (auto mesh : SubMeshList)
//	{
//		//MeshList.emplace_back(Mesh(renderer, mesh.VertexList, mesh.IndexList, mesh.TextureList));
//	}
//}
//
//Model::~Model()
//{
//
//}
//
//void Model::UpdateUniformBuffer(Renderer& renderer, PositionMatrix ubo2, MeshProp viewpos)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.UpdateUniformBuffer(renderer, ubo2, viewpos);
//	}
//}
//
//void Model::Draw(Renderer& renderer, int currentImage)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.Draw(renderer, currentImage);
//	}
//}
//
//void Model::Destroy(Renderer& renderer)
//{
//	for (auto mesh : MeshList)
//	{
//		mesh.Destory(renderer);
//	}
//}
