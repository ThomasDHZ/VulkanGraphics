//#pragma once
//#include <vector>
//#include "Mesh.h"
//#include "Vertex.h"
//#include "Texture2D.h"
//
//struct SubMesh
//{
//	std::vector<Vertex> VertexList;
//	std::vector<uint16_t> IndexList;
//	std::vector<Texture2D> TextureList;
//};
//
//class ModelLoader : VulkanResources
//{
//private:
//
//
//	void LoadModel(Renderer& renderer, const std::string& FilePath);
//	void ProcessNode(Renderer& renderer, const std::string& FilePath, aiNode* node, const aiScene* scene);
//	std::vector<Vertex> LoadVertices(aiMesh* mesh);
//	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
//	std::vector<Texture2D> LoadTextures(Renderer& renderer, const std::string& FilePath, aiMesh* mesh, const aiScene* scene);
//
//public:
//	ModelLoader();
//	ModelLoader(Renderer& renderer, const std::string& FilePath);
//	~ModelLoader();
//
//	std::vector<SubMesh> ModelMeshList;
//	std::vector<SubMesh> GetModelMeshs() { return ModelMeshList; }
//	void CleanTextureMemory(Renderer& renderer);
//};
//
