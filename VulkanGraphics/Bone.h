#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Vertex.h"
#include <assimp\scene.h>

struct Bone
{

	unsigned int BoneID = -1;
	std::string BoneName = "";
	glm::mat4 OffsetMatrix = glm::mat4(1.0f);
	glm::mat4 FinalTransformMatrix = glm::mat4(1.0f);

	Bone()
	{
	}
	Bone(const std::string& boneName, int boneId, glm::mat4 offsetMatrix)
	{
		BoneID = boneId;
		BoneName = boneName;
		OffsetMatrix = offsetMatrix;
		FinalTransformMatrix = glm::mat4(1.0f);
	}
};

