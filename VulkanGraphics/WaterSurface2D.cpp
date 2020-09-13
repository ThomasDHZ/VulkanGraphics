#include "WaterSurface2D.h"

WaterSurface2D::WaterSurface2D() : Sprite()
{
}

WaterSurface2D::WaterSurface2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, glm::vec2 StartPos, glm::vec2 WaterSize, std::shared_ptr<Texture> texture) : Sprite()
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
	CoinTextures.RendererDiffuseMap = texture;
	CoinTextures.SpecularMap = "texture/coin_specularOriginal.bmp";
	CoinTextures.NormalMap = "texture/fn03a-default-noise-1024x720.jpg";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	RenderBitFlags = RenderBitFlag::RenderOnMainPass;
	ObjectFlagBits = ObjectFlags::None;

	SetUpSprite(renderer, textureManager, Water2DVertices, CoinTextures, StartPos, custom);
}

WaterSurface2D::~WaterSurface2D()
{
}

void WaterSurface2D::DrawMessage(RendererManager& renderer)
{
	ObjectMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.reflection2DPipeline);
}

void WaterSurface2D::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	ObjectMesh->Update(renderer, camera, light, static_cast<void*>(&waveprop));
	const glm::vec3 BottomLeftVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

void WaterSurface2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
}
