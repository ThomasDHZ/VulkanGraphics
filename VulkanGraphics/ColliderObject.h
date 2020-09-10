#pragma once
#include "BoxCollider.h"
enum CollidorType 
{
	LevelCollider,
	EnemyCollider
};

class ColliderObject
{
private:
	glm::vec3 Position = glm::vec3(0.0f);
	BoxCollider Collider;
	CollidorType Type;
public:
	ColliderObject();
	ColliderObject(BoxCollider collider, CollidorType type);
	ColliderObject(glm::vec2 position, CollidorType type);
	ColliderObject(glm::vec3 position, CollidorType type);
	~ColliderObject();

	void Update(glm::vec2 position);
	void Update(glm::vec3 position);

	BoxCollider GetCollider() { return Collider; }
};

