#include "Water2D.h"

Water2D::Water2D()
{
}

Water2D::Water2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize)
{
	const std::vector<Vertex> Water2DVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.4f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, 0.0f, 0.0f},			 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, WaterSize.y, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, WaterSize.y, 0.0f},		     {0.0f, 0.0f, 1.0f}, {0.4f, 0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> Water2DIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	MeshTextures CoinTextures = {};
	CoinTextures.DiffuseMap = "texture/coin_diffuseOriginal.bmp";
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/coin_normal.bmp";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	//DrawMessage draw = {};
	//draw.RenderBit = RendererBitFlag::RenderOnMainPass;
	//draw.pipeline = renderer.forwardRenderer.reflection2DPipeline;
	//draw.mesh = SpriteMesh;
	//auto a = std::make_shared<DrawMessage>(draw);
	//renderer.AddDrawableMesh(a);

	Type = SpriteType::SCoin;
	ObjectFlagBits = ObjectFlags::Collectible;
	SpriteMaps = CoinTextures;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, Water2DVertices, Water2DIndices, CoinTextures, descriptorSetLayout, RendererBitFlag::RenderOnMainPass));
	renderer.AddDrawableMesh(SpriteMesh);
	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

Water2D::Water2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize, Texture& texture)
{
	const std::vector<Vertex> Water2DVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, 0.0f, 0.0f},			 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, WaterSize.y, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, WaterSize.y, 0.0f},		     {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	const std::vector<uint16_t> Water2DIndices =
	{
		  0, 1, 2, 2, 3, 0
	};

	MeshTextures CoinTextures = {};
	CoinTextures.DiffuseMap = "texture/coin_diffuseOriginal.bmp";
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/fn03a-default-noise-1024x720.jpg";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	Type = SpriteType::SCoin;
	ObjectFlagBits = ObjectFlags::Collectible;
	SpriteMaps = CoinTextures;
	SpriteMesh = std::make_shared<Mesh2D>(Mesh2D(renderer, textureManager, Water2DVertices, Water2DIndices, CoinTextures, descriptorSetLayout, RendererBitFlag::RenderOnMainPass, texture));
	SpriteMesh->reflect = true;

	//DrawMessage draw = {};
	//draw.RenderBit = RendererBitFlag::RenderOnMainPass;
	//draw.pipeline = renderer.forwardRenderer.reflection2DPipeline;
	//draw.mesh = SpriteMesh;
	//auto a = std::make_shared<DrawMessage>(draw);
	//renderer.AddDrawableMesh(a);

	renderer.AddDrawableMesh(SpriteMesh);
	SetPosition2D(StartPos);

	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

Water2D::~Water2D()
{
}

void Water2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
}
