#pragma once
#include <vector>
#include "Mesh.h"
#include "ModelLoader.h"

class Model : VulkanResources
{
private:


public:
	Model();
	Model(Renderer& renderer, const std::vector<SubMesh>& SubMeshList);
	~Model();

	std::vector<Mesh> MeshList;

	std::string ModelName;
	glm::vec3 ModelPosition = glm::vec3(0.0f);
	glm::vec3 ModelRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	void UpdateUniformBuffer(Renderer& renderer, Camera& camera, Lights& viewpos);
	void Draw(Renderer& renderer, int currentImage);
	void Destroy(Renderer& renderer);

	std::vector<Mesh> GetModelMeshList()
	{
		return MeshList;
	}
};

