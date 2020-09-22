#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Vertex.h"
#include "Texture2D.h"

struct SubMesh
{
	std::vector<Vertex> VertexList;
	std::vector<uint16_t> IndexList;
	std::vector<Texture2D> TextureList;
};

class ModelLoader
{
private:
	std::vector<SubMesh> ModelMeshList;

	void LoadModel(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath);
	void ProcessNode(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
	std::vector<Texture2D> LoadTextures(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

public:
	ModelLoader();
	ModelLoader(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath);
	~ModelLoader();

	std::vector<SubMesh> GetModelMeshs() { return ModelMeshList; }
};

