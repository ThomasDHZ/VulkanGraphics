#include "Cube.h"

Cube::Cube() : Mesh()
{
}

Cube::Cube(VulkanRenderer Renderer, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, const std::vector<Texture2D>& textureList) : Mesh(Renderer, vertices, indices, textureList)
{
	MeshPosition = Position;
	MeshRotate = Rotation;
	MeshScale = Scale;
}

Cube::~Cube()
{
}
