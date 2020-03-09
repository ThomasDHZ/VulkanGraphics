#pragma once
#include <vector>
#include "Mesh.h"

struct SubMesh
{
	std::vector<Vertex> VertexList;
	std::vector<unsigned int> IndexList;
	std::vector<Texture2D> TextureList;
};

class ModelLoader
{
private:
	VulkanDevice DeviceInfo;
	std::vector<SubMesh> ModelMeshList;

	void LoadModel(const std::string& FilePath);
	void ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<unsigned int> LoadIndices(aiMesh* mesh);
	std::vector<Texture2D> LoadTextures(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

public:
	ModelLoader();
	ModelLoader(VulkanDevice deviceInfo, const std::string& FilePath);
	~ModelLoader();

	std::vector<SubMesh> GetModelMeshs() { return ModelMeshList; }
};

