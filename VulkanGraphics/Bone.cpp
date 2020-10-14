#include "Bone.h"

Bone::Bone()
{
	OffsetMatrix = glm::mat4(1.0f);
	BoneTransformMatrix = glm::mat4(1.0f);
}

Bone::Bone(const std::string& boneName, int boneId, glm::mat4 offsetMatrix)
{
	BoneID = boneId;
	BoneName = boneName;
	OffsetMatrix = offsetMatrix; 
	BoneTransformMatrix = glm::mat4(1.0f);
}

Bone::~Bone()
{
}

void Bone::AddChildBone(Bone bone)
{
	ChildrenBoneList.emplace_back(bone);
}

void Bone::InvertBoneMatrix(Bone& BoneList, const glm::mat4& ParentTransformMatrix, const glm::mat4& GlobalInverseTransformMatrix)
{
	glm::mat4 GlobalTransform = ParentTransformMatrix * this->OffsetMatrix;
	BoneTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * this->OffsetMatrix;

	for (int x = 0; x < BoneList.ChildrenBoneList.size(); x++)
	{
		InvertBoneMatrix(this->ChildrenBoneList[x], this->BoneTransformMatrix, GlobalInverseTransformMatrix);
	}
}
