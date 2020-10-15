#include "Bone.h"
#include <iostream>

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
	glm::mat4 GlobalTransform = ParentTransformMatrix * glm::mat4(1.0f);
	BoneTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * BoneList.OffsetMatrix;

	std::cout << "Parent" << std::endl;
	std::cout << ParentTransformMatrix[0].x << "  " << ParentTransformMatrix[0].y << "  " << ParentTransformMatrix[0].z << "  " << ParentTransformMatrix[0].w << std::endl;
	std::cout << ParentTransformMatrix[1].x << "  " << ParentTransformMatrix[1].y << "  " << ParentTransformMatrix[1].z << "  " << ParentTransformMatrix[1].w << std::endl;
	std::cout << ParentTransformMatrix[2].x << "  " << ParentTransformMatrix[2].y << "  " << ParentTransformMatrix[2].z << "  " << ParentTransformMatrix[2].w << std::endl;
	std::cout << ParentTransformMatrix[3].x << "  " << ParentTransformMatrix[3].y << "  " << ParentTransformMatrix[3].z << "  " << ParentTransformMatrix[3].w << std::endl;

	std::cout << "Offset" << std::endl;
	std::cout << BoneList.OffsetMatrix[0].x << "  " << BoneList.OffsetMatrix[0].y << "  " << BoneList.OffsetMatrix[0].z << "  " << BoneList.OffsetMatrix[0].w << std::endl;
	std::cout << BoneList.OffsetMatrix[1].x << "  " << BoneList.OffsetMatrix[1].y << "  " << BoneList.OffsetMatrix[1].z << "  " << BoneList.OffsetMatrix[1].w << std::endl;
	std::cout << BoneList.OffsetMatrix[2].x << "  " << BoneList.OffsetMatrix[2].y << "  " << BoneList.OffsetMatrix[2].z << "  " << BoneList.OffsetMatrix[2].w << std::endl;
	std::cout << BoneList.OffsetMatrix[3].x << "  " << BoneList.OffsetMatrix[3].y << "  " << BoneList.OffsetMatrix[3].z << "  " << BoneList.OffsetMatrix[3].w << std::endl;


	std::cout << "Global" << std::endl;
	std::cout << GlobalTransform[0].x << "  " << GlobalTransform[0].y << "  " << GlobalTransform[0].z << "  " << GlobalTransform[0].w << std::endl;
	std::cout << GlobalTransform[1].x << "  " << GlobalTransform[1].y << "  " << GlobalTransform[1].z << "  " << GlobalTransform[1].w << std::endl;
	std::cout << GlobalTransform[2].x << "  " << GlobalTransform[2].y << "  " << GlobalTransform[2].z << "  " << GlobalTransform[2].w << std::endl;
	std::cout << GlobalTransform[3].x << "  " << GlobalTransform[3].y << "  " << GlobalTransform[3].z << "  " << GlobalTransform[3].w << std::endl;

	std::cout << "GlobalInverseTransformMatrix" << std::endl;
	std::cout << GlobalInverseTransformMatrix[0].x << "  " << GlobalInverseTransformMatrix[0].y << "  " << GlobalInverseTransformMatrix[0].z << "  " << GlobalInverseTransformMatrix[0].w << std::endl;
	std::cout << GlobalInverseTransformMatrix[1].x << "  " << GlobalInverseTransformMatrix[1].y << "  " << GlobalInverseTransformMatrix[1].z << "  " << GlobalInverseTransformMatrix[1].w << std::endl;
	std::cout << GlobalInverseTransformMatrix[2].x << "  " << GlobalInverseTransformMatrix[2].y << "  " << GlobalInverseTransformMatrix[2].z << "  " << GlobalInverseTransformMatrix[2].w << std::endl;
	std::cout << GlobalInverseTransformMatrix[3].x << "  " << GlobalInverseTransformMatrix[3].y << "  " << GlobalInverseTransformMatrix[3].z << "  " << GlobalInverseTransformMatrix[3].w << std::endl;


	std::cout << BoneList.BoneName << std::endl;
	std::cout << BoneTransformMatrix[0].x << "  " << BoneTransformMatrix[0].y << "  " << BoneTransformMatrix[0].z << "  " << BoneTransformMatrix[0].w << std::endl;
	std::cout << BoneTransformMatrix[1].x << "  " << BoneTransformMatrix[1].y << "  " << BoneTransformMatrix[1].z << "  " << BoneTransformMatrix[1].w << std::endl;
	std::cout << BoneTransformMatrix[2].x << "  " << BoneTransformMatrix[2].y << "  " << BoneTransformMatrix[2].z << "  " << BoneTransformMatrix[2].w << std::endl;
	std::cout << BoneTransformMatrix[3].x << "  " << BoneTransformMatrix[3].y << "  " << BoneTransformMatrix[3].z << "  " << BoneTransformMatrix[3].w << std::endl;


	for (int x = 0; x < BoneList.ChildrenBoneList.size(); x++)
	{
		InvertBoneMatrix(BoneList.ChildrenBoneList[x], GlobalTransform, GlobalInverseTransformMatrix);
	}
}
