#pragma once
#include <vector>
#include "Mesh.h"
#include "ModelLoader.h"

class Model : VulkanResources
{
private:
	std::vector<Mesh> MeshList;

public:
	Model();
	Model(Renderer& renderer, const std::vector<SubMesh>& SubMeshList);
	~Model();

	std::string ModelName;
	glm::vec3 ModelPosition = glm::vec3();
	glm::vec3 ModelRotate = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 ModelScale = glm::vec3(1.0f);

	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix ubo2, MeshProp viewpos);
	void Draw(Renderer& renderer, int currentImage);
	void Destroy(Renderer& renderer);

	std::vector<Mesh> GetModelMeshList()
	{
		return MeshList;
	}
};

