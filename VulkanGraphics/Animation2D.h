#pragma once
#include <vector>
#include <glm/glm.hpp>

class Animation2D
{
private:
	float lastframeTime;
	unsigned int CurrentFrame;
	float TimeBetweenFrames;
	std::vector<glm::vec2> UVOffsetList;

public:
	Animation2D();
	Animation2D(std::vector<glm::vec2> AnimationFrames, float timeBetweenFrames);
	void Update();

	glm::vec2 GetCurrentFrame() { return UVOffsetList[CurrentFrame]; }
};

