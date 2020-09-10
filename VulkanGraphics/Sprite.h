#pragma once
#include "RendererManager.h"
#include "Mesh2D.h"
#include "Animation2D.h"
#include <map>
#include "BoxCollider.h"
#include "ColliderObject.h"

struct ColisionGeo
{
	std::vector<glm::vec3> CollisionVertexs;
	std::vector<uint16_t> CollisionIndices;
};

enum ObjectFlags
{
	None = 0x00,
	Player = 0x01,
	Enemy = 0x02,
	Wall = 0x04,
	Collectible = 0x08,
	ApplyGravity = 0x10
};

class Sprite
{
private:

protected:
	void SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos);
	void SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, CustomBuffer custom);
	virtual void DrawMessage(RendererManager& renderer);

	static constexpr glm::vec3 Gravity = glm::vec3(0.0f, -0.01f, 0.0f);
public:

	glm::ivec2 Velocity;
	int ObjectFlagBits;
	int RenderBitFlags;
	Animation2D CurrentAni;
	BoxCollider collider;
	std::shared_ptr<Mesh2D> SpriteMesh;
	glm::vec2 UVOffset = glm::vec2(0.0f);

	Sprite();
	~Sprite();


	void ApplyGravity(std::vector<std::shared_ptr<Sprite>> SpriteList);
	void ApplyGravity(std::vector<std::shared_ptr<ColliderObject>> TileColliderList);
	void Move(std::vector<std::shared_ptr<Sprite>> SpriteList, glm::vec3 MoveDirection);
	void Move(std::vector<std::shared_ptr<ColliderObject>> TileColliderList, glm::vec3 MoveDirection);
	virtual void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light);
	virtual void AnimationHandler();
	virtual void Collision(RendererManager& renderer, std::vector<std::shared_ptr<Sprite>> SpriteList) = 0;
	void Destory(RendererManager& renderer);

	bool OnGroundCheck(std::vector<std::shared_ptr<ColliderObject>> TileColliderList);

	void SetPosition2D(glm::vec2 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition3D(float x, float y, float z);

	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh->GetPosition2D().x, SpriteMesh->GetPosition2D().y); }
	glm::vec3 GetPosition3D() { return SpriteMesh->GetPosition3D(); }
	float* GetUVOffsetPtr() { return &UVOffset.x; };
};