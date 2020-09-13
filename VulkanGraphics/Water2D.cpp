#include "Water2D.h"

Water2D::Water2D() : Sprite()
{
}

Water2D::Water2D(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, glm::vec2 StartPos, glm::vec2 WaterSize, const OrthographicCamera& camera, const std::shared_ptr<Texture> texture) : Sprite()
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
	CoinTextures.AlphaMap = "texture/SparkManAlpha.bmp";

	RenderBitFlags = RenderBitFlag::RenderOnMainPass;
	ObjectFlagBits = ObjectFlags::None;

	WaterCamera = OrthographicCamera(camera.Width, camera.Height);
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

void Water2D::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	ObjectMesh->Update(renderer, camera, light, static_cast<void*>(&waveprop));
	const glm::vec3 BottomLeftVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[0].Position;
	const glm::vec3 BottomRightVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[1].Position;
	const glm::vec3 TopRightVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[2].Position;
	const glm::vec3 TopLeftVertex = ObjectMesh.get()->GetPosition3D() + ObjectMesh.get()->Vertexdata[3].Position;
	collider = BoxCollider(TopLeftVertex.x, TopRightVertex.x, TopRightVertex.y, BottomRightVertex.y);
}

void Water2D::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{

}
