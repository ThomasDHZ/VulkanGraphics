#include "Sprite.h"
#include "LevelSprite.h"



Sprite::Sprite() : Object2D()
{
}

Sprite::Sprite(RendererManager& renderer, int renderBitFlags) : Object2D()
{
}

Sprite::~Sprite()
{
}

bool Sprite::OnGroundCheck(std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
	for (auto& obj : ObjectList)
	{
		for (auto objCollider : obj->ObjectColliderList)
		{
			for (auto thisObjCollider : ObjectColliderList)
			{
				if (thisObjCollider->GetCollider().CollidesWith(objCollider->GetCollider(), Gravity))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Sprite::ApplyGravity(std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
	glm::vec3 MoveDirection = Gravity;
	if (ObjectFlagBits & ObjectFlags::ApplyGravity)
	{
		for (auto& obj : ObjectList)
		{
			if (auto Level = dynamic_cast<LevelSprite*>(obj.get()))
			{
				for (auto& collider : obj->ObjectColliderList)
				{
					if (collider->GetCollider().CollidesWith(ObjectColliderList[0]->GetCollider(), MoveDirection))
					{
						MoveDirection = glm::vec3(0.0f);
						break;
					}
				}
			}
		}
		ObjectMesh->MeshPosition += MoveDirection;
	}
}

void Sprite::Move(std::vector<std::shared_ptr<Object2D>>& ObjectList, glm::vec3 MoveDirection)
{
	if (MoveDirection != glm::vec3(0.0f))
	{
		for (auto& obj : ObjectList)
		{
			/*if (obj.get() != this)
			{
				if (auto Level = dynamic_cast<LevelSprite*>(obj.get()))
				{

				}
				else
				{
					for (auto& collider : obj->ObjectColliderList)
					{
						if (collider->GetCollider().CollidesWith(ObjectColliderList[0]->GetCollider(), MoveDirection))
						{
							MoveDirection = glm::vec3(0.0f);
							break;
						}
					}
				}
			}*/
		}
		ObjectMesh->MeshPosition += MoveDirection;
	}
}

void Sprite::SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos)
{
	const std::vector<uint16_t> SpriteIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	ObjectMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, SpriteVertices, SpriteIndices, SpriteTextures));
	ObjectColliderList.emplace_back(std::make_shared<ColliderObject>(SpriteVertices, SpriteIndices, glm::vec3(StartPos.x, StartPos.y, ObjectMesh->GetPosition3D().z), CollidorType::EnemyCollider));

	SetPosition2D(StartPos);
	DrawMessage(renderer);
}

void Sprite::SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, CustomBuffer custom)
{
	const std::vector<uint16_t> SpriteIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	ObjectMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, SpriteVertices, SpriteIndices, SpriteTextures, custom));
	ObjectColliderList.emplace_back(std::make_shared<ColliderObject>(SpriteVertices, SpriteIndices, glm::vec3(StartPos.x, StartPos.y, ObjectMesh->GetPosition3D().z), CollidorType::EnemyCollider));

	SetPosition2D(StartPos);
	DrawMessage(renderer);
}

void Sprite::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	CurrentAni.Update();
	ObjectMesh->properites.UVOffset = CurrentAni.GetCurrentFrame().GetUVOffset();
	ObjectMesh->properites.UVScale = CurrentAni.GetCurrentFrame().GetUVScale();

	Object2D::Update(renderer, camera, light);
}

void Sprite::Destory(RendererManager& renderer)
{
	vkDeviceWaitIdle(renderer.GetVulkanRendererBase()->Device);
	ObjectMesh->Destory(renderer);
}