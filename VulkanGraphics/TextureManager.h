#pragma once
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
class TextureManager
{
private:
	std::vector<Texture> TextureList;

	unsigned int IDNum = 0;
	unsigned int CreateNewTextureID();

public:
	TextureManager();
	TextureManager(VulkanRenderer& renderer);
	~TextureManager();

	unsigned int LoadTexture(VulkanRenderer& renderer, std::string TextureLocation, VkFormat format);
	unsigned int LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList);
	unsigned int LoadTexture(Texture texture);
	void UnloadTexture(VulkanRenderer& renderer, unsigned int ID);
	void UnloadAllTextures(VulkanRenderer& renderer);
	void UpdateIMGUIVRAM();

	bool GetTextureByName(std::string name);
	bool GetTextureByName(std::string name, unsigned int& textureID);
	Texture GetTextureByID(unsigned int ID);

	Texture GetTexture(int index) { return TextureList[index]; }
	std::vector<Texture> GetTextureList() {  return TextureList; }
};

