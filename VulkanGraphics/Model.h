#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <unordered_map>
#include <Animation3D.h>
#include <AnimationPlayer3D.h>

class Model
{
private:
	//const unsigned int MAX_BONE_VERTEX_COUNT = 4;

	std::vector<Mesh> MeshList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
	std::vector<NodeMap> NodeMapList;
	glm::mat4 GlobalInverseTransformMatrix;

	std::vector<MeshData> SubMeshList;


	AnimationPlayer3D AnimationPlayer;
	Animation3D CurrentAnimation;

	void LoadModel(VulkanEngine& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath);
	void ProcessNode(VulkanEngine& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<uint16_t> LoadIndices(aiMesh* mesh);
	void LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList);
	void LoadNodeTree(const aiNode* Node, int parentNodeID);
	void LoadAnimations(const aiScene* scene);
	MeshTextures LoadTextures(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiMesh* mesh, const aiScene* scene);
	void SendDrawMessage(RendererManager& renderer);

	void BoneWeightPlacement(std::vector<Vertex>& VertexList, unsigned int vertexID, unsigned int bone_id, float weight);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);

public:
	Model();
	Model(RendererManager& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath);
	~Model();

	std::string ModelName;
	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	void UpdateUniformBuffer(RendererManager& renderer, std::shared_ptr<Camera>& camera, LightBufferObject& light);
	void Destroy(RendererManager& renderer);

	std::vector<Mesh> GetModelMeshList()
	{
		return MeshList;
	}
};
