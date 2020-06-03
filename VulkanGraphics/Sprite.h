#pragma once
#include "Mesh.h"

class Sprite : public Mesh
{
private:
public:

	Sprite();
	Sprite(Renderer& renderer);
	~Sprite();

	void UpdateSpriteUVs(Renderer& renderer);
};

