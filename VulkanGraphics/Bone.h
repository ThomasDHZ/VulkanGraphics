#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Vertex.h"

class Bone
{
private:
	int BoneID;
	std::string BoneName;
	std::vector<Bone> ChildrenBoneList;
	glm::mat4 OffsetMatrix;
	glm::mat4 BoneTransformMatrix;

public:
	Bone();
	Bone(const std::string& boneName, int boneId, glm::mat4 boneTransformMatrix);
	~Bone();

	void AddChildBone(Bone bone);
	void InvertBoneMatrix(Bone& BoneList, const glm::mat4& ParentTransformMatrix, const glm::mat4& GlobalInverseTransformMatrix);

	int GetBoneID() { return BoneID; }
	const std::string& GetBoneName() { return BoneName; }
	const std::vector<Bone> GetChildrenBoneList() { return ChildrenBoneList; }
	const glm::mat4 GetOffsetMatrix() { return OffsetMatrix; }
	const glm::mat4 GetBoneTransformMatrix() { return BoneTransformMatrix; }
};

