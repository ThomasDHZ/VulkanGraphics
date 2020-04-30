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
	void SetUpCubeMapImage(VulkanRenderer& Renderer, CubeMapLayout CubeMapFiles);
	void CreateTextureSampler(VulkanRenderer& Renderer);
public:
	CubeMapTexture();
	CubeMapTexture(VulkanRenderer& Renderer, CubeMapLayout CubeMapFiles);
	~CubeMapTexture();
};

