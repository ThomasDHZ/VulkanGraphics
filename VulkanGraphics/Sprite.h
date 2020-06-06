#pragma once
#include "Mesh.h"

class Sprite
{
private:
public:

	Mesh SpriteMesh;

	Sprite();
	Sprite(Renderer& renderer, glm::vec2 StartPos);
	Sprite(Renderer& renderer, glm::vec3 StartPos);
	~Sprite();

	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix, MeshProp viewpos);
	void Draw(Renderer& renderer, int currentFrame);
	void UpdateSpriteUVs(Renderer& renderer);
	
	void SetPosition2D(glm::vec2 Pos);
	void SetPosition2D(float x, float y);
	void SetPosition3D(glm::vec3 Pos);
	void SetPosition3D(float x, float y, float z);

	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh.MeshPosition.x, SpriteMesh.MeshPosition.y); }
	glm::vec3 GetPosition3D() { return SpriteMesh.MeshPosition; }
};

