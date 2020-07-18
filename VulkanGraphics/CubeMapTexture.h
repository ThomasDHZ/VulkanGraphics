#pragma once
#include "Texture.h"

struct CubeMapLayout
{
	std::string Front;
	std::string Back;
	std::string Top;
	std::string Bottom;
	std::string Right;
	std::string Left;
};

class CubeMapTexture : public Texture
{
private:
	void SetUpCubeMapImage(Renderer& renderer, CubeMapLayout CubeMapFiles);
	void CreateTextureSampler(Renderer& renderer);
public:
	CubeMapTexture();
	CubeMapTexture(Renderer& renderer, CubeMapLayout CubeMapFiles);
	~CubeMapTexture();
};

