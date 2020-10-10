#pragma once
#include <memory>
#include "Bone.h"

class Animation3D
{
private:
	std::shared_ptr<std::vector<Bone>> Skeleton;

public:
	float AnimationTime;
	float TicksPerSec;

	Animation3D();
	Animation3D(const std::shared_ptr<std::vector<Bone>> skeleton);
	~Animation3D();

};
