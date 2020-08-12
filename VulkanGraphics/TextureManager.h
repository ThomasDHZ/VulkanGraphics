#pragma once
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
class TextureManager
{
private:
	std::vector<Texture> TextureList;

	void CreateNewTextureID();

public:
	TextureManager();
	TextureManager(VulkanRenderer& renderer);
	~TextureManager();

	void LoadTexture(VulkanRenderer& renderer, std::string TextureLocation);
	void LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList);
	void LoadTexture(Texture texture);
	void UnloadTexture(VulkanRenderer& renderer, unsigned int ID);
	void UnloadAllTextures(VulkanRenderer& renderer);
	void UpdateIMGUIVRAM();

	bool GetTextureByName(std::string name);
	Texture GetTextureByID(unsigned int ID);

	Texture GetTexture(int index) { return TextureList[index]; }
	std::vector<Texture> GetTextureList() {  return TextureList; }
};

