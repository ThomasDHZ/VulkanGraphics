#pragma once
#include "Mesh.h"
#include <map>
#include "Animation2D.h"

enum SpriteAnime
{
	Stand1,
	Stand2,
	Stand3,
	Run1,
	Run2,
	Run3,
	Run4
};

class Sprite
{
private:

	Animation2D StandAni;
	Animation2D RunAni;

public:
	Animation2D CurrentAni;

	TextureMaps maps;
	std::map<SpriteAnime, glm::vec2> AnimationFrame;
	Mesh SpriteMesh;
	glm::vec2 UVOffset = glm::vec2(0.0f);

	Sprite();
	Sprite(Renderer& renderer, glm::vec2 StartPos);
	Sprite(Renderer& renderer, glm::vec3 StartPos);
	~Sprite();

	void UpdateUniformBuffer(GLFWwindow* window, Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos);
	void Draw(Renderer& renderer, int currentFrame);
	void Destory(Renderer& renderer);

	void SetPosition2D(glm::vec2 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition3D(float x, float y, float z);

	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh.MeshPosition.x, SpriteMesh.MeshPosition.y); }
	glm::vec3 GetPosition3D() { return SpriteMesh.MeshPosition; }
	float* GetUVOffsetPtr() { return &UVOffset.x; };
};

