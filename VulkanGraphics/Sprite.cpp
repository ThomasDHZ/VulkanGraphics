#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(Renderer& renderer, glm::vec2 StartPos)
{
	TextureMaps maps;
	maps.DiffuseMap = Texture2D(renderer, "texture/MegaManDiffuse2048.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/MegaManSpecular2048.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/MegaManAlpha2048.bmp");

	const unsigned int TileSize = 2048;
	const float AmtXAxisTiles = maps.DiffuseMap.Width / TileSize;
	const float AmtYAxisTiles = maps.DiffuseMap.Height / TileSize;
	const float UVTileLocU = 1 / AmtXAxisTiles;
	const float UVTileLocV = 1 / AmtYAxisTiles;

	AnimationFrame[Stand1] = glm::vec2(0.0f, 0.0f);
	AnimationFrame[Stand2] = glm::vec2(0.138f, 0.0f);
	AnimationFrame[Stand3] = glm::vec2(0.275f, 0.0f);
	AnimationFrame[Run1] = glm::vec2(0.431f, 0.0f);
	AnimationFrame[Run2] = glm::vec2(0.569f, 0.0f);
	AnimationFrame[Run3] = glm::vec2(0.706f, 0.0f);
	AnimationFrame[Run4] = glm::vec2(0.826f, 0.0f);


	std::vector<glm::vec2> StandFrames = { AnimationFrame[Stand1] , AnimationFrame[Stand2] , AnimationFrame[Stand3] };
	std::vector<glm::vec2> RunFrames = { AnimationFrame[Run1] , AnimationFrame[Run2], AnimationFrame[Run3], AnimationFrame[Run4] };

	StandAni = Animation2D(StandFrames, 0.25f);
	RunAni = Animation2D(RunFrames, 0.1f);
	CurrentAni = RunAni;

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
	CurrentAni.Update();
	viewpos.SpriteUV = glm::vec3(CurrentAni.GetCurrentFrame(), 0.0f);

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
