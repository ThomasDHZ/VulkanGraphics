#include "WaterSurface2D.h"

WaterSurface2D::WaterSurface2D()
{
}

WaterSurface2D::WaterSurface2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, VkDescriptorSetLayout& descriptorSetLayout, glm::vec2 StartPos, glm::vec2 WaterSize, Texture& texture)
{
	CustomBuffer custom = {};
	custom.ByteSize = sizeof(WaveProperites);

	const std::vector<Vertex> Water2DVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, 0.0f, 0.0f},			 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{WaterSize.x, WaterSize.y, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, WaterSize.y, 0.0f},		     {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	MeshTextures CoinTextures = {};
	CoinTextures.DiffuseMap = "texture/coin_diffuseOriginal.bmp";
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/fn03a-default-noise-1024x720.jpg";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	RenderBitFlags = RenderBitFlag::RenderOnMainPass;
	ObjectFlagBits = ObjectFlags::None;

	SetUpSprite(renderer, textureManager, Water2DVertices, CoinTextures, StartPos, texture, custom);
}

WaterSurface2D::~WaterSurface2D()
{
}

void WaterSurface2D::DrawMessage(RendererManager& renderer)
{
	auto drawMessage = SpriteMesh->CreateDrawMessage(1, renderer.forwardRenderer.reflection2DPipeline);
	renderer.DrawMessageList.emplace_back(drawMessage);
}

void WaterSurface2D::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	SpriteMesh->Update(renderer, camera, light, static_cast<void*>(&waveprop));
	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

void WaterSurface2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
}
