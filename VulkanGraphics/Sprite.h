#pragma once
#include "Object2D.h"
#include "RendererManager.h"
#include "Mesh2D.h"
#include "Animation2D.h"
#include <map>
#include "BoxCollider.h"
#include "ColliderObject.h"


class Sprite : public Object2D
{
private:

protected:
	void SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos);
	void SetUpSprite(RendererManager& renderer, std::shared_ptr<TextureManager> textureManager, const std::vector<Vertex> SpriteVertices, const MeshTextures& SpriteTextures, glm::vec2 StartPos, CustomBuffer custom);

public:

	glm::ivec2 Velocity;
	Animation2D CurrentAni;

	Sprite();
	Sprite(RendererManager& renderer, int renderBitFlags);
	~Sprite();

	bool OnGroundCheck(std::vector<std::shared_ptr<Object2D>>& ObjectList);
	void ApplyGravity(std::vector<std::shared_ptr<Object2D>>& ObjectList, float dt);
	void Move(std::vector<std::shared_ptr<Object2D>>& ObjectList, glm::vec3 MoveDirection);

	virtual void Update(RendererManager& renderer, float dt, std::shared_ptr<Camera> camera, LightBufferObject light) override;
	virtual void Destory(RendererManager& renderer) override;
};