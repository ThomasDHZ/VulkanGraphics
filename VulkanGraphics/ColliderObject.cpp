#include "ColliderObject.h"

ColliderObject::ColliderObject()
{
}

ColliderObject::ColliderObject(BoxCollider collider, CollidorType type)
{
	Collider = collider;
	Type = type;
}

ColliderObject::ColliderObject(glm::vec2 position, CollidorType type)
{
	Position = glm::vec3(position.x, position.y, Position.z);
	Type = type;
}

ColliderObject::ColliderObject(glm::vec3 position, CollidorType type)
{
	Position = position;
	Type = type;
}

ColliderObject::~ColliderObject()
{
}

void ColliderObject::Update(glm::vec2 position)
{
}

void ColliderObject::Update(glm::vec3 position)
{
}
