#include "Bone.h"

Bone::Bone()
{
}

Bone::Bone(const std::string& boneName, int index, glm::mat4 bindLocalTransform)
{
	BoneName = boneName;
	BoneID = index;
	LocalBindTransform = bindLocalTransform;
}

Bone::~Bone()
{
}

void Bone::AddChildBone(Bone bone)
{
	ChildrenBoneList.emplace_back(bone);
}

void Bone::CalcInverseBindTransform(Bone* parentBone)
{
	glm::mat4 TempMatrix = parentBone->GetLocalBindTransform() * LocalBindTransform;
	InverseBindTransform = glm::inverse(TempMatrix);
	
	for (auto& childBone : ChildrenBoneList)
	{
		childBone.CalcInverseBindTransform(this);
	}
}

void Bone::SetBoneTransform(glm::mat4 Transform)
{
	BoneTransformMatrix = Transform;
}