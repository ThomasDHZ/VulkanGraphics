#include "AnimationPlayer3D.h"
#include <GLFW\glfw3.h>

aiVector3D AnimationPlayer3D::InterpolatePosition(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BonePosition.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BonePosition[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BonePosition.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].Time) / delta_time;

	aiVector3D StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[Frame].AnimationInfo;
	aiVector3D EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BonePosition[NextFrame].AnimationInfo;
	aiVector3D Diffrence = EndPos - StartPos;

	return StartPos + factor * Diffrence;
}

aiQuaternion AnimationPlayer3D::InterpolateRotation(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BoneRotation.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BoneRotation[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BoneRotation.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].Time) / delta_time;

	aiQuaternion StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[Frame].AnimationInfo;
	aiQuaternion EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneRotation[NextFrame].AnimationInfo;

	return nlerp(StartPos, EndPos, factor);
}

aiVector3D AnimationPlayer3D::InterpolateScaling(const std::shared_ptr<Bone> bone, float AnimationTime, const int NodeID)
{
	int Frame = 0;
	for (int x = CurrentAnimation.BoneKeyFrameList[0].BoneScale.size() - 1; x > 0; x--)
	{
		if (AnimationTime >= CurrentAnimation.BoneKeyFrameList[0].BoneScale[x].Time)
		{
			Frame = x;
			break;
		}
	}

	int NextFrame = Frame + 1;
	if (NextFrame >= CurrentAnimation.BoneKeyFrameList[0].BoneScale.size() - 1)
	{
		NextFrame = 0;
	}

	float delta_time = (float)(CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[NextFrame].Time - CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].Time);
	float factor = (AnimationTime - (float)CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].Time) / delta_time;

	aiVector3D StartPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[Frame].AnimationInfo;
	aiVector3D EndPos = CurrentAnimation.BoneKeyFrameList[bone->BoneID].BoneScale[NextFrame].AnimationInfo;
	aiVector3D Diffrence = EndPos - StartPos;

	return StartPos + factor * Diffrence;
}

void AnimationPlayer3D::UpdateSkeleton(const int NodeID, const glm::mat4 ParentMatrix)
{
	glm::mat4 glmTransform = AssimpToGLMMatrixConverter(NodeMapList[NodeID].NodeTransform);

	auto Time = (float)glfwGetTime() * CurrentAnimation.TicksPerSec;
	float AnimationTime = fmod(Time, CurrentAnimation.AnimationTime);

	for (auto bone : Skeleton)
	{
		if (NodeMapList[NodeID].NodeString == bone->BoneName)
		{
			aiMatrix4x4 ScaleMatrix;
			aiMatrix4x4 TranslateMatrix;

			aiVector3D scaling_vector = InterpolateScaling(bone, AnimationTime, NodeID);
			aiMatrix4x4::Scaling(scaling_vector, ScaleMatrix);

			aiQuaternion rotate_quat = InterpolateRotation(bone, AnimationTime, NodeID);
			aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

			aiVector3D translate_vector = InterpolatePosition(bone, AnimationTime, NodeID);
			aiMatrix4x4::Translation(translate_vector, TranslateMatrix);

			glmTransform = AssimpToGLMMatrixConverter(TranslateMatrix * rotate_matr * ScaleMatrix);
		}
	}

	glm::mat4 GlobalTransform = ParentMatrix * glmTransform;

	for (auto bone : Skeleton)
	{
		if (NodeMapList[NodeID].NodeString == bone->BoneName)
		{
			bone->FinalTransformMatrix = GlobalInverseTransformMatrix * GlobalTransform * bone->OffsetMatrix;
		}
	}

	for (int x = 0; x < NodeMapList[NodeID].ChildNodeList.size(); x++)
	{
		UpdateSkeleton(NodeMapList[NodeID].ChildNodeList[x], GlobalTransform);
	}
}

glm::mat4 AnimationPlayer3D::AssimpToGLMMatrixConverter(aiMatrix4x4 matrix)
{
	return glm::mat4();
}

aiQuaternion AnimationPlayer3D::nlerp(aiQuaternion quaternion1, aiQuaternion quaternion2, float blend)
{
	quaternion1.Normalize();
	quaternion2.Normalize();

	aiQuaternion result;
	float dot_product = quaternion1.x * quaternion2.x + quaternion1.y * quaternion2.y + quaternion1.z * quaternion2.z + quaternion1.w * quaternion2.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = quaternion1.x * one_minus_blend + blend * -quaternion2.x;
		result.y = quaternion1.y * one_minus_blend + blend * -quaternion2.y;
		result.z = quaternion1.z * one_minus_blend + blend * -quaternion2.z;
		result.w = quaternion1.w * one_minus_blend + blend * -quaternion2.w;
	}
	else
	{
		result.x = quaternion1.x * one_minus_blend + blend * quaternion2.x;
		result.y = quaternion1.y * one_minus_blend + blend * quaternion2.y;
		result.z = quaternion1.z * one_minus_blend + blend * quaternion2.z;
		result.w = quaternion1.w * one_minus_blend + blend * quaternion2.w;
	}

	return result.Normalize();
}

AnimationPlayer3D::AnimationPlayer3D()
{
}
