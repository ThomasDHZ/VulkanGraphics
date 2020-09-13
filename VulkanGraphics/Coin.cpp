#include "Coin.h"
#include "MegaMan.h"
#include <algorithm>
#include <functional>

Coin::Coin() : Sprite()
{
}

Coin::Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	const std::vector<Vertex> SpriteVertices =
	{
		{{0.0f, 0.0f, -0.1f},				 {0.0f, 0.0f, 1.0f}, {0.4f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, -0.1f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, -0.1f},		 {0.0f, 0.0f, 1.0f}, {0.4f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	std::vector<Frame2D> StandFrames;
	StandFrames.emplace_back(Frame2D(glm::vec2(0.0f, 0.0f)));
	StandFrames.emplace_back(Frame2D(glm::vec2(0.0521f, 0.0f)));
	StandFrames.emplace_back(Frame2D(glm::vec2(0.1021f, 0.0f)));

	CoinTurn = Animation2D(StandFrames, 1.0f, 0);
	CurrentAni = CoinTurn;

	MeshTextures CoinTextures = {};
	CoinTextures.DiffuseMap = "texture/coin_diffuseOriginal.bmp";
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/coin_normal.bmp";
	CoinTextures.AlphaMap = "texture/coin_alpha.bmp";

	ObjectFlagBits = ObjectFlags::Collectible;
	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass;

	SetUpSprite(renderer, textureManager, SpriteVertices, CoinTextures, StartPos);
}

Coin::~Coin()
{
}

void Coin::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
	//auto spriteList = SpriteList;
	//spriteList.erase(std::remove_if(SpriteList.begin(), SpriteList.end(),
	//		[](const std::shared_ptr<Sprite> spriteList)
	//		{
	//			 return spriteList->ObjectFlagBits & ObjectFlags::Collectible;
	//		}));


	//for (auto sprite : SpriteList)
	//{
	//	if(dynamic_cast<MegaMan*>(sprite.get()))
	//	{
	//		for (auto spriteCollider : sprite->ObjectColliderList)
	//		{
	//			if (spriteCollider->GetCollider().CollidesWith(spriteCollider->GetCollider()))
	//			{
	//				Destory(renderer);
	//			}
	//		}
	//	}
	//}
}
