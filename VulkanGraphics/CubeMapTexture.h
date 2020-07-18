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
	void SetUpCubeMapImage(VulkanRenderer& renderer, CubeMapLayout CubeMapFiles);
	void CreateTextureSampler(VulkanRenderer& renderer);
public:
	CubeMapTexture();
	CubeMapTexture(VulkanRenderer& renderer, CubeMapLayout CubeMapFiles);
	~CubeMapTexture();
};

