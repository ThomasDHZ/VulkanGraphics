#include "Coin.h"

Coin::Coin()
{
}

Coin::Coin(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos) : Sprite()
{
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

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.4f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.4f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> MegaManIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	Type = SpriteType::SCoin;
	ObjectFlagBits = ObjectFlags::Collectible;
	SpriteMaps = CoinTextures;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, MegaManVertices, MegaManIndices, CoinTextures, descriptorSetLayout, RendererBitFlag::RenderOnMainPass | RendererBitFlag::RenderShadow | RendererBitFlag::RenderOnTexturePass));

	//DrawMessage draw = {};
	//draw.RenderBit = RendererBitFlag::RenderOnMainPass;
	//draw.pipeline = renderer.forwardRenderer.renderer2DPipeline;
	//draw.mesh = SpriteMesh;
	//auto a = std::make_shared<DrawMessage>(draw);
	////renderer.AddDrawableMesh(a);

	//draw.RenderBit = RendererBitFlag::RenderOnTexturePass;
	//draw.pipeline = renderer.forwardRenderer.renderer2DPipeline;
	//draw.mesh = SpriteMesh;
	////renderer.AddDrawableMesh(a);

	renderer.AddDrawableMesh(SpriteMesh);
	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

Coin::~Coin()
{
}

void Coin::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
	for (auto sprite : SpriteList)
	{
		if (sprite->Type == SpriteType::SMegaMan)
		{
			if(sprite->collider.CollidesWith(collider))
			{
				Destory(renderer);
			}
		}
	}
}
