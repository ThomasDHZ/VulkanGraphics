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
	glm::mat4 BoneTransformMatrix;

public:
	Bone();
	Bone(int index, const std::string& boneName);
	~Bone();

	void AddChildBone(Bone bone);

	int GetBoneID() { return BoneID; }
	const std::string& GetBoneName() { return BoneName; }
	const std::vector<Bone> GetChildrenBoneList() { return ChildrenBoneList; }
	const glm::mat4 GetBoneTransformMatrix() { return BoneTransformMatrix; }
};

