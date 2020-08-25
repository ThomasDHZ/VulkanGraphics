#pragma once
#include "LevelMesh2D.h"
#include "Animation2D.h"
#include <map>

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
	SMMShot
};

class Sprite
{
private:
	MeshTextures SpriteMaps;

public:

	SpriteType Type;
	Animation2D CurrentAni;

	std::map<SpriteAnime, glm::vec2> AnimationFrame;
	LevelMesh2D SpriteMesh;
	glm::vec2 UVOffset = glm::vec2(0.0f);
	ColisionGeo CollisionVertexs;

	Sprite();
	Sprite(VulkanRenderer& renderer, std::shared_ptr<TextureManager>textureManager, float Width, float Height, MeshTextures spriteMaps, glm::vec2 StartPos, SpriteType type, VkDescriptorSetLayout& descriptorSetLayout, int renderBit);
	Sprite(VulkanRenderer& renderer, float Width, float Height, MeshTextures spriteMaps, glm::vec3 StartPos, SpriteType type);
	~Sprite();

	virtual void Update(VulkanRenderer& renderer, OrthographicCamera& camera, LightBufferObject light);
	//void Draw(VulkanRenderer& renderer, int currentFrame);
	void Destory(VulkanRenderer& renderer);

	void SetPosition2D(glm::vec2 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition3D(float x, float y, float z);

	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh.MeshPosition.x, SpriteMesh.MeshPosition.y); }
	glm::vec3 GetPosition3D() { return SpriteMesh.MeshPosition; }
	float* GetUVOffsetPtr() { return &UVOffset.x; };
};

