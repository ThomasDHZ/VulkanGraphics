#pragma once
#include "RendererManager.h"
#include "Mesh2D.h"
#include "Animation2D.h"
#include <map>
#include "BoxCollider.h"

enum SpriteAnime
{
	Stand1,
	Stand2,
	StartRun,
	Run1,
	Run2,
	Run3,
	Run4
};

struct ColisionGeo
{
	std::vector<glm::vec3> CollisionVertexs;
	std::vector<uint16_t> CollisionIndices;
};

enum SpriteType
{
	SMegaMan,
	SMMShot,
	SCoin
};

class Sprite
{
private:


public:
	MeshTextures SpriteMaps;
	glm::ivec2 Velocity;
	SpriteType Type;
	Animation2D CurrentAni;
	BoxCollider collider;
	std::map<SpriteAnime, glm::vec2> AnimationFrame;
	std::shared_ptr<Mesh2D> SpriteMesh;
	glm::vec2 UVOffset = glm::vec2(0.0f);
	//ColisionGeo CollisionVertexs;

	Sprite();
	Sprite(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, VkDescriptorSetLayout& descriptorSetLayout, SpriteType type, int renderBit);
	Sprite(RendererManager& renderer, std::shared_ptr<TextureManager>textureManager, float Width, float Height, MeshTextures spriteMaps, glm::vec2 StartPos, SpriteType type, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
	Sprite(RendererManager& renderer, float Width, float Height, MeshTextures spriteMaps, glm::vec3 StartPos, SpriteType type);
	~Sprite();

	virtual void Update(RendererManager& renderer, OrthographicCamera& camera, LightBufferObject light);
	virtual void Collision(std::vector<Sprite>& SpriteList);
	//void Draw(VulkanRenderer& renderer, int currentFrame);
	void Destory(RendererManager& renderer);

	void SetPosition2D(glm::vec2 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition3D(float x, float y, float z);

	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh->MeshPosition.x, SpriteMesh->MeshPosition.y); }
	glm::vec3 GetPosition3D() { return SpriteMesh->MeshPosition; }
	float* GetUVOffsetPtr() { return &UVOffset.x; };
};

