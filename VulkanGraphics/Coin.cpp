#include "Coin.h"
#include "MegaMan.h"

Coin::Coin()
{
}

Coin::Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	const std::vector<Vertex> SpriteVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.4f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.4f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	AnimationFrame[CoinFlip1] = glm::vec2(0.011f, 0.0f);
	AnimationFrame[CoinFlip2] = glm::vec2(0.144f, 0.0f);
	AnimationFrame[CoinFlip3] = glm::vec2(0.279f, 0.0f);
	AnimationFrame[CoinFlip4] = glm::vec2(0.422f, 0.0f);
	AnimationFrame[CoinFlip5] = glm::vec2(0.568f, 0.0f);
	AnimationFrame[CoinFlip6] = glm::vec2(0.700f, 0.0f);

	std::vector<glm::vec2> CoinTurn2 = { AnimationFrame[CoinFlip1], AnimationFrame[CoinFlip2], AnimationFrame[CoinFlip3], AnimationFrame[CoinFlip4], AnimationFrame[CoinFlip5], AnimationFrame[CoinFlip6] };

	CoinTurn = Animation2D(CoinTurn2, 1.0f, 0);
	CurrentAni = CoinTurn;

	MeshTextures CoinTextures = {};
	CoinTextures.DiffuseMap = "texture/coin_diffuseOriginal.bmp";
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/coin_normal.bmp";
	CoinTextures.AlphaMap = "texture/coin_alpha.bmp";

	SetUpSprite(renderer, textureManager, SpriteVertices, CoinTextures, StartPos);
}

Coin::~Coin()
{
}

void Coin::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	for (auto sprite : SpriteList)
	{
		if(dynamic_cast<MegaMan*>(sprite.get()))
		{
			if(sprite->collider.CollidesWith(collider))
			{
				Destory(renderer);
			}
		}
	}
}
