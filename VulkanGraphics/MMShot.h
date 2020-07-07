#pragma once
#include "Sprite.h"

class MMShot : public Sprite
{
private:
	Animation2D StandAni;
	Animation2D RunAni;
public:
	MMShot();
	MMShot(Renderer& renderer, TextureMaps SpriteMaps, glm::vec2 StartPos);
	MMShot(Renderer& renderer, TextureMaps SpriteMaps, glm::vec3 StartPos);
	~MMShot();

	void Update(GLFWwindow* window, Renderer& renderer, Camera& camera, Lights light);
};

