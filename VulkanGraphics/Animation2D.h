#pragma once
#include <vector>
#include <glm/glm.hpp>

class Animation2D
{
private:
	unsigned int AnimationID;
	float lastframeTime;
	unsigned int CurrentFrame;
	float TimeBetweenFrames;
	std::vector<glm::vec2> UVOffsetList;

public:
	Animation2D();
	Animation2D(std::vector<glm::vec2> AnimationFrames, float timeBetweenFrames, unsigned int animationID);
	void Update();

	unsigned int GetAnimationID() { return AnimationID; }
	glm::vec2 GetCurrentFrame() { return UVOffsetList[CurrentFrame]; }
};

