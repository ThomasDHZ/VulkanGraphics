#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& descriptorSetLayout, SpriteType type, int ObjectFlagBits, int renderBit)
{

}

Sprite::Sprite(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, float Width, float Height, MeshTextures spriteMaps, glm::vec2 StartPos, SpriteType type, VkDescriptorSetLayout& descriptorSetLayout, int ObjectFlagBits, int renderBit)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{Width, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{Width, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = type;
	SpriteMaps = spriteMaps;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, MegaManVertices, MegaManIndices, SpriteMaps, descriptorSetLayout, renderBit));
	renderer.AddDrawableMesh(SpriteMesh);
	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

Sprite::Sprite(RendererManager& renderer, float Width, float Height, MeshTextures spriteMaps, glm::vec3 StartPos, SpriteType type, int ObjectFlagBits)
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{Width, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{Width, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, Height, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
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

void Sprite::Gravity(std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	if (ObjectFlagBits & ObjectFlags::ApplyGravity)
	{
		glm::vec3 MoveDirection = glm::vec3(0.0f, -0.01f, 0.0f);
		Move(SpriteList, MoveDirection);
	}
}

void Sprite::Gravity(std::vector<BoxCollider> SpriteList)
{
	if (ObjectFlagBits & ObjectFlags::ApplyGravity)
	{
		glm::vec3 MoveDirection = glm::vec3(0.0f, -0.01f, 0.0f);
		Move(SpriteList, MoveDirection);
	}
}

void Sprite::Move(std::vector<std::shared_ptr<Sprite>> SpriteList, glm::vec3 MoveDirection)
{
	for (auto& sprite : SpriteList)
	{
		if (sprite.get() != this &&
			sprite->ObjectFlagBits & ObjectFlags::Wall)
		{
			if (collider.CollidesWith(sprite->collider, MoveDirection))
			{
				MoveDirection = glm::vec3(0.0f);
				break;
			}
		}
	}

	SpriteMesh->MeshPosition += MoveDirection;
}

void Sprite::Move(std::vector<BoxCollider> SpriteList, glm::vec3 MoveDirection)
{
	for (auto& sprite : SpriteList)
	{

			if (collider.CollidesWith(sprite, MoveDirection))
			{
				MoveDirection = glm::vec3(0.0f);
				break;
			}
		
	}

	SpriteMesh->MeshPosition += MoveDirection;
}

void Sprite::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	//CurrentAni.Update();
	//SpriteMesh->properites.UVOffset = glm::vec2(CurrentAni.GetCurrentFrame());

	SpriteMesh->Update(renderer, camera, light);
	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}


//void Sprite::Draw(VulkanRenderer& renderer, int currentFrame)
//{
//	///SpriteMesh.Draw(renderer, currentFrame);
//}

void Sprite::Destory(RendererManager& renderer)
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);
	SpriteMesh->Destory(renderer);
	renderer.RemoveMesh(SpriteMesh);
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
