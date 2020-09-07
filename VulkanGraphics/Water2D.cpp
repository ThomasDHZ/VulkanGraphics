#include "Water2D.h"

Water2D::Water2D()
{
}

Water2D::Water2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, glm::vec2 StartPos, glm::vec2 WaterSize, const std::shared_ptr<Texture> texture)
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
	CoinTextures.SpecularMap = "texture/SparkManAlpha.bmp";
	CoinTextures.NormalMap = "texture/SparkManAlpha.bmp";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	RenderBitFlags = RenderBitFlag::RenderOnMainPass;
	ObjectFlagBits = ObjectFlags::None;

	WaterCamera = OrthographicCamera(WaterSize);
	WaterCamera.SetPosition(StartPos);
	SetUpSprite(renderer, textureManager, Water2DVertices, CoinTextures, StartPos, texture, custom);
}

Water2D::~Water2D()
{
}

void Water2D::DrawMessage(RendererManager& renderer)
{
	SpriteMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.underwater2DPipeline);
}

void Water2D::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	SpriteMesh->Update(renderer, camera, light, static_cast<void*>(&waveprop));
	const glm::vec3 BottomLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = SpriteMesh.get()->MeshPosition + SpriteMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

void Water2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList)
{
}
