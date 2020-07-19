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
//	TextureMaps TextureList;
//};
//
//class ModelLoader : VulkanResources
//{
//private:
//	std::vector<SubMesh> ModelMeshList;
//
//	void LoadModel(Renderer& renderer, const std::string& FilePath);
//	void ProcessNode(Renderer& renderer, const std::string& FilePath, aiNode* node, const aiScene* scene);
//	std::vector<Vertex> LoadVertices(aiMesh* mesh);
//	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
//	TextureMaps LoadTextures(Renderer& renderer, const std::string& FilePath, aiMesh* mesh, const aiScene* scene);
//
//public:
//	ModelLoader();
//	ModelLoader(Renderer& renderer, const std::string& FilePath);
//	~ModelLoader();
//
//	std::vector<SubMesh> GetModelMeshs() { return ModelMeshList; }
//	void CleanTextureMemory(Renderer& renderer);
//};
//
