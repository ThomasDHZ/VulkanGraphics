#include "Enemy2D.h"
#include "MMShot.h"

Enemy2D::Enemy2D(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, -0.1f},				 {0.0f, 0.0f, 1.0f}, {0.0521f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.x, -0.1f}, {0.0f, 0.0f, 1.0f}, {0.0f, -1.0f},  {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.x, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.0521f, -1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "texture/MegaMan_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "texture/MegaMan_Specular.bmp";
	MegaManTextures.NormalMap = "texture/MegaMan_Normal.bmp";
	MegaManTextures.AlphaMap = "texture/MegaMan_Alpha.bmp";

	ObjectFlagBits = ObjectFlags::Player | ObjectFlags::ApplyGravity;
	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass;

	SetUpSprite(renderer, textureManager, MegaManVertices, MegaManTextures, StartPos);
}


Enemy2D::~Enemy2D()
{
}

void Enemy2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
	for (auto& obj : ObjectList)
	{
		if (dynamic_cast<MMShot*>(obj.get()))
		{
			for (auto& collider : obj->ObjectColliderList)
			{
				if (collider->GetCollider().CollidesWith(ObjectColliderList[0]->GetCollider()))
				{
					HP--;
					
					obj->DestoryObjectFlag = true;
					if (HP <= 0)
					{
						DestoryObjectFlag = true;
						break;
					}
				}
			}
		}
	}
}