#include "MMShot.h"
MMShot::MMShot() : Sprite()
{
}

MMShot::MMShot(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, glm::vec2 StartPos) : Sprite()
{
	MeshTextures MegaManTextures = {};
	MegaManTextures.DiffuseMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/MegaManShot_diffuseOriginal.bmp";
	MegaManTextures.SpecularMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/MegaManShot_Specular.bmp";
	MegaManTextures.NormalMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/MegaManShot_normal.bmp";
	MegaManTextures.AlphaMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/MegaManShot_alpha.bmp";
	MegaManTextures.EmissionMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/texture/MegaManShot_diffuseOriginal.bmp";

	const std::vector<Vertex> MegaManVertices =
	{
		{{0.0f, 0.0f, 0.0f},				 {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, 0.0f, 0.0f},		 {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{SpriteSize.x, SpriteSize.y, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
		{{0.0f, SpriteSize.y, 0.0f},		 {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
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
