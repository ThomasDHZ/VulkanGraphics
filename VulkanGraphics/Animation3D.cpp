#include "Animation3D.h"

Animation3D::Animation3D()
{
}

Animation3D::Animation3D(const std::shared_ptr<std::vector<Bone>> skeleton)
{
	Skeleton = skeleton;
}

Animation3D::~Animation3D()
{
}
