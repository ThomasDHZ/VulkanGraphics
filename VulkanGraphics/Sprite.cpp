#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, float Width, float Height, MeshTextures spriteMaps, glm::vec2 StartPos, SpriteType type, VkDescriptorSetLayout& descriptorSetLayout, int renderBit)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{Width, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{Width, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{0.0f, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = type;
	SpriteMaps = spriteMaps;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, MegaManVertices, MegaManIndices, SpriteMaps, descriptorSetLayout, renderBit));
	SetPosition2D(StartPos);
}

Sprite::Sprite(VulkanRenderer& renderer, float Width, float Height, MeshTextures spriteMaps, glm::vec3 StartPos, SpriteType type)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
		{{Width, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
		{{Width, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{0.0f, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = type;
	SpriteMaps = spriteMaps;
	//SpriteMesh = Mesh(renderer, MegaManVertices, MegaManIndices, SpriteMaps);
	SetPosition2D(StartPos);
}

Sprite::~Sprite()
{
}

void Sprite::Update(VulkanRenderer& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	CurrentAni.Update();
	SpriteMesh->properites.UVOffset = glm::vec2(CurrentAni.GetCurrentFrame());

	SpriteMesh->Update(renderer, camera, light);
}

//void Sprite::Draw(VulkanRenderer& renderer, int currentFrame)
//{
//	///SpriteMesh.Draw(renderer, currentFrame);
//}

void Sprite::Destory(VulkanRenderer& renderer)
{
	SpriteMesh->Destory(renderer);
}

void Sprite::SetPosition2D(glm::vec2 Pos)
{
	SpriteMesh->MeshPosition = glm::vec3(Pos, 0.0f);
}

void Sprite::SetPosition2D(float x, float y)
{
	SpriteMesh->MeshPosition = glm::vec3(x, y, 0.0f);
}

void Sprite::SetPosition3D(glm::vec3 Pos)
{
	SpriteMesh->MeshPosition = Pos;
}

void Sprite::SetPosition3D(float x, float y, float z)
{
	SpriteMesh->MeshPosition = glm::vec3(x, y, z);
}
