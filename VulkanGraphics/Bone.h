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
	glm::mat4 LocalBindTransform;
	glm::mat4 InverseBindTransform;
	
	void AddChildBone(Bone bone);
	void CalcInverseBindTransform(Bone* parentBone);
public:
	
	Bone();
	Bone(const std::string& boneName, int index, glm::mat4 bindLocalTransform);
	~Bone();

	void SetBoneTransform(glm::mat4 Transform);

	glm::mat4 GetBoneTransformMatrix() { return BoneTransformMatrix; }
	glm::mat4 GetLocalBindTransform() { return LocalBindTransform; }
	glm::mat4 GetInverseBindTransform() { return InverseBindTransform; }
};

