#pragma once
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
class TextureManager
{
private:
	std::vector<Texture> TextureList;
	std::vector<CubeMapTexture> CubeMapTextureList;
public:
	TextureManager();
	TextureManager(VulkanRenderer& renderer);
	~TextureManager();

	void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation);
	void LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList);
	void LoadTexture(Texture texture);
	void UnloadTexture(VulkanRenderer& renderer);
	void UnloadAllTextures(VulkanRenderer& renderer);
	void UpdateIMGUIVRAM();

	Texture GetTexture(int index) { return TextureList[index]; }
	CubeMapTexture GetCubeMapTexture(int index) { return CubeMapTextureList[index]; }
	std::vector<Texture> GetTextureList() {  return TextureList; }
	std::vector<CubeMapTexture> GetCubeMapTextureList() { return CubeMapTextureList; }
};

