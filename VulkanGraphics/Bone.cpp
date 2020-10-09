#include "Bone.h"

Bone::Bone()
{
}

Bone::Bone(int index, const std::string& boneName)
{
	BoneID = index;
	BoneName = boneName;
}

Bone::~Bone()
{
}

void Bone::AddChildBone(Bone bone)
{
	ChildrenBoneList.emplace_back(bone);
}
