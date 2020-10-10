#include "Bone.h"

Bone::Bone()
{
}

Bone::Bone(const std::string& boneName, int boneId, glm::mat4 boneTransformMatrix)
{
	BoneID = boneId;
	BoneName = boneName;
	BoneTransformMatrix = boneTransformMatrix;
	BoneTransformMatrix[1][1] *= -1;
}

Bone::~Bone()
{
}

void Bone::AddChildBone(Bone bone)
{
	ChildrenBoneList.emplace_back(bone);
}