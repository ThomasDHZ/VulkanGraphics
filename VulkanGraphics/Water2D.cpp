#include "Water2D.h"

Water2D::Water2D() : Sprite()
{
}

Water2D::Water2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, glm::vec2 StartPos, glm::vec2 WaterSize, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Texture> texture) : Sprite()
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
	CoinTextures.SpecularMap = "texture/SparkManAlpha.bmp";
	CoinTextures.NormalMap = "texture/SparkManAlpha.bmp";
	CoinTextures.EmissionMap = "texture/SparkManAlpha.bmp";
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass | RenderBitFlag::RenderMainPass | RenderBitFlag::RenderEffectPass;
	ObjectFlagBits = ObjectFlags::None;

	WaterCamera = OrthographicCamera(glm::vec2(1920, 1080));
	WaterCamera.SetPosition(StartPos);
	SetUpSprite(renderer, textureManager, Water2DVertices, CoinTextures, StartPos, custom);
}

Water2D::~Water2D()
{
}

void Water2D::DrawMessage(RendererManager& renderer)
{
	ObjectMesh->CreateDrawMessage(renderer, 1, renderer.forwardRenderer.underwater2DPipeline);
}

