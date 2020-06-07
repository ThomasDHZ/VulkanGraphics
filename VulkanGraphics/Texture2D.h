#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
private:
	void CreateTextureSampler(Renderer& renderer);

public:

	Texture2D();
	Texture2D(Renderer& renderer, std::string TexturePath);
	~Texture2D();
};

