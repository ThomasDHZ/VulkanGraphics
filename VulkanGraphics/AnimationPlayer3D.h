#pragma once
#include <Animation3D.h>

struct NodeMap
{
	std::string NodeString;
	aiMatrix4x4 NodeTransform;
	int ParentNodeID;
	int NodeID;
	std::vector<int> ChildNodeList;
};

class AnimationPlayer3D
{
private:
	Animation3D CurrentAnimation;
	std::vector<std::shared_ptr<Bone>> Skeleton;

	std::vector<NodeMap> NodeMapList;
	glm::mat4 GlobalInverseTransformMatrix;

	aiVector3D InterpolatePosition(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiQuaternion InterpolateRotation(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);
	aiVector3D InterpolateScaling(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID);

	void UpdateSkeleton(const int NodeID, const glm::mat4 ParentMatrix);

	glm::mat4 AssimpToGLMMatrixConverter(aiMatrix4x4 matrix);
	aiQuaternion nlerp(aiQuaternion quaternion1, aiQuaternion quaternion2, float blend);

public:
	AnimationPlayer3D();
};

