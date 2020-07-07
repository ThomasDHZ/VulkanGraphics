#include "Animation2D.h"
#include <chrono>

Animation2D::Animation2D()
{
	CurrentFrame = 0;
	TimeBetweenFrames = 0;
}

Animation2D::Animation2D(std::vector<glm::vec2> AnimationFrames, float timeBetweenFrames, unsigned int animationID)
{
	AnimationID = animationID;
	CurrentFrame = 0;
	lastframeTime = 0;
	TimeBetweenFrames = timeBetweenFrames;
	UVOffsetList = AnimationFrames;
}

void Animation2D::Update()
{
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	if ((time - lastframeTime) > TimeBetweenFrames)
	{
		if (CurrentFrame < UVOffsetList.size() - 1)
		{
			CurrentFrame++;
		}
		else
		{
			CurrentFrame = 0;
		}
		lastframeTime = time;
	}
}
