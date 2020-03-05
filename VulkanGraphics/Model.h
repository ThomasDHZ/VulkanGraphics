#pragma once
#include <vector>
#include "Mesh.h"
class Model
{
private:
	VulkanDevice DeviceInfo;
	MainPipeline Pipeline;
	std::vector<Mesh> MeshList;

	void ModelLoader(const std::string& FilePath);
	void ProcessNode(const std::string& FilePath, aiNode* node, const aiScene* scene);
	std::vector<Vertex> LoadVertices(aiMesh* mesh);
	std::vector<unsigned int> LoadIndices(aiMesh* mesh);
	std::vector<Texture2D> LoadTextures(const std::string& FilePath, aiMesh* mesh, const aiScene* scene);

public:
	Model();
	Model(VulkanDevice deviceInfo, const std::string& FilePath, MainPipeline Pipeline);
	~Model();

	void UpdateUniformBuffer(UniformBufferObject2 ubo2, int currentImage);
	void Draw(VkCommandBuffer commandbuffer, MainPipeline pipeline, int currentImage);
};

