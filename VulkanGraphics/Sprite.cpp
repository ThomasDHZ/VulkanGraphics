#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(Renderer& renderer, glm::vec2 StartPos)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.11f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.11f, 1.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	TextureMaps maps;
	maps.DiffuseMap = Texture2D(renderer, "texture/MegaManDiffuse2048.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/MegaManSpecular2048.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/MegaManAlpha2048.bmp");

	SpriteMesh = Mesh(renderer, MegaManVertices, MegaManIndices, maps);
	SetPosition2D(StartPos);
}

Sprite::Sprite(Renderer& renderer, glm::vec3 StartPos)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.11f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.11f, 1.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	TextureMaps maps;
	maps.DiffuseMap = Texture2D(renderer, "texture/MegaManDiffuse2048.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/MegaManSpecular2048.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/MegaManAlpha2048.bmp");

	SpriteMesh = Mesh(renderer, MegaManVertices, MegaManIndices, maps);
	SetPosition2D(StartPos);
}

Sprite::~Sprite()
{
}

void Sprite::UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos)
{
	SpriteMesh.UpdateUniformBuffer(renderer, positionMatrix, viewpos);
}

void Sprite::Draw(Renderer& renderer, int currentFrame)
{
	SpriteMesh.Draw(renderer, currentFrame);
}

void Sprite::UpdateSpriteUVs(Renderer& renderer)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.55f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.55f, 1.0f}}
	};
	SpriteMesh.VertexList = MegaManVertices;
	SpriteMesh.UpdateSpriteUVs(renderer);
}

void Sprite::SetPosition2D(glm::vec2 Pos)
{
	SpriteMesh.MeshPosition = glm::vec3(Pos, 0.0f);
}

void Sprite::SetPosition2D(float x, float y)
{
	SpriteMesh.MeshPosition = glm::vec3(x, y, 0.0f);
}

void Sprite::SetPosition3D(glm::vec3 Pos)
{
	SpriteMesh.MeshPosition = Pos;
}

void Sprite::SetPosition3D(float x, float y, float z)
{
	SpriteMesh.MeshPosition = glm::vec3(x, y, z);
}
