#include "MMShot.h"
MMShot::MMShot() : Sprite()
{
}

MMShot::MMShot(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "texture/MegaMan_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "texture/container2_specular.png";
	MegaManTextures.NormalMap = "texture/SparkMan_normal.bmp";
	MegaManTextures.AlphaMap = "texture/SparkManAlpha.bmp";
	MegaManTextures.EmissionMap = "texture/MegaMan_Emission.bmp";

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.14f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.14f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass | RenderBitFlag::RenderMainPass ;

	SetUpSprite(renderer, textureManager, MegaManVertices, MegaManTextures, StartPos);
}

MMShot::~MMShot()
{
}

void MMShot::Update(RendererManager& renderer, float dt, std::shared_ptr<Camera> camera, LightBufferObject light)
{
	ObjectMesh->MeshPosition.x += 7.0f * dt;
	Sprite::Update(renderer, dt, camera, light);
	if (ObjectMesh->GetPosition2D().x - SpriteSize.x > static_cast<OrthographicCamera*>(camera.get())->GetViewScreenSize().x + static_cast<OrthographicCamera*>(camera.get())->GetPosition().x)
	{
		DestoryObjectFlag = true;
	}
}

void MMShot::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
}
