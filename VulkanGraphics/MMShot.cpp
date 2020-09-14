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

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {0.14f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.14f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
	};

	RenderBitFlags = RenderBitFlag::RenderOnTexturePass | RenderBitFlag::RenderOnMainPass;

	SetUpSprite(renderer, textureManager, MegaManVertices, MegaManTextures, StartPos);
}

MMShot::~MMShot()
{
}

void MMShot::Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light)
{
	ObjectMesh->MeshPosition.x += 0.1f;
	Sprite::Update(renderer, camera, light);
	if (ObjectMesh->GetPosition2D().x - SpriteSize.x > camera.GetViewScreenSize().x + camera.GetPosition().x)
	{
		Destory(renderer);
	}
}

void MMShot::Collision(RendererManager& renderer, std::vector<std::shared_ptr<Object2D>>& ObjectList)
{
}
