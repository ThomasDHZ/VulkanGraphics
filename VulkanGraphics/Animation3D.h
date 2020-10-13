#pragma once
#include <memory>
#include "Bone.h"

struct KeyFrameInfo
{
	float Time;
	glm::vec3 AnimationInfo;
};
struct KeyFrame
{
	int BoneId;
	std::string BoneName;
	std::vector<KeyFrameInfo> BonePosition;
	std::vector<KeyFrameInfo> BoneRotation;
	std::vector<KeyFrameInfo> BoneScale;
};

class Animation3D
{
private:
	std::shared_ptr<std::vector<Bone>> Skeleton;
	std::vector<KeyFrame> BoneKeyFrameList;

public:
	float AnimationTime;
	float TicksPerSec;

	Animation3D();
	Animation3D(const std::shared_ptr<std::vector<Bone>> skeleton);
	~Animation3D();

	void AddBoneKeyFrame(KeyFrame keyFrame);
};
