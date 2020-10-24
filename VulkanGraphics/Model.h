#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <unordered_map>
#include <Animation3D.h>

struct NodeMap
{
	std::string NodeString;
	aiMatrix4x4 NodeTransform;
	int ParentNodeID;
	int NodeID;
	std::vector<int> ChildNodeList;
};

class Model
{
private:
	//const unsigned int MAX_BONE_VERTEX_COUNT = 4;

	std::vector<Mesh> MeshList;
	std::vector<std::shared_ptr<Bone>> BoneList;
	std::vector<Animation3D> AnimationList;
	std::vector<NodeMap> NodeMapList;

	std::vector<MeshData> SubMeshList;

	glm::mat4 GlobalInverseTransformMatrix;
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

	aiVector3D InterpolatePosition(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiQuaternion InterpolateRotation(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiVector3D InterpolateScaling(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);

	void BoneWeightPlacement(std::vector<Vertex>& VertexList, unsigned int vertexID, unsigned int bone_id, float weight);
	void UpdateSkeleton(const int NodeID, const glm::mat4 ParentMatrix);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);
	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);

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
