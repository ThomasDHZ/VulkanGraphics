#include "BoxCollider.h"

BoxCollider::BoxCollider()
{
}

BoxCollider::BoxCollider(float left, float right, float top, float bottom)
{
	Left = left;
	Right = right;
	Top = top;
	Bottom = bottom;
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::CollidesWith(const BoxCollider& OtherBox) const
{
	bool a = Right < OtherBox.Left;
	bool b = Left < OtherBox.Right;
	bool c = Bottom > OtherBox.Top;
	bool d = Top > OtherBox.Bottom;
	bool e = Right < OtherBox.Left&&
		Left < OtherBox.Right&&
		Bottom > OtherBox.Top&&
		Top > OtherBox.Bottom;
	return Right > OtherBox.Left &&
		   Left < OtherBox.Right &&
		   Bottom < OtherBox.Top &&
		   Top > OtherBox.Bottom;
}

bool BoxCollider::CollidesWith(const BoxCollider& OtherBox, const glm::vec3 NewPos) const
{

	return Right + NewPos.x > OtherBox.Left &&
		   Left + NewPos.x < OtherBox.Right&&
		   Bottom + NewPos.y < OtherBox.Top&&
		   Top + NewPos.y > OtherBox.Bottom;
}
