#pragma once
#include "Texture2.h"

struct CubeMapLayout
{
	std::string Front;
	std::string Back;
	std::string Top;
	std::string Bottom;
	std::string Right;
	std::string Left;
};

class NewCubeMapTexture : public Texture2
{
private:
    void LoadTexture(VulkanRenderer& renderer, CubeMapLayout CubeMapFiles);
    void CreateTextureView(VulkanRenderer& renderer);
    void CreateTextureSampler(VulkanRenderer& renderer);
public:
    NewCubeMapTexture();
    NewCubeMapTexture(VulkanRenderer& renderer, CubeMapLayout CubeMapFiles);
    ~NewCubeMapTexture();
};