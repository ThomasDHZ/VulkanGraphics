#pragma once
#include "Texture.h"
#include "Mesh.h"

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
	void SetUpCubeMapImage(CubeMapLayout CubeMapFiles);
	void CreateTextureSampler();
public:
	CubeMapTexture();
	CubeMapTexture(VulkanRenderer* renderer, CubeMapLayout CubeMapFiles);
	~CubeMapTexture();
};

