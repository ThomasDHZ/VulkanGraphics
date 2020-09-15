#pragma once
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "CubeMapTexture.h"
class TextureManager
{
private:
	std::vector<std::shared_ptr<Texture>> TextureList;

	unsigned int IDNum = 0;
	unsigned int CreateNewTextureID();

public:
	TextureManager();
	TextureManager(VulkanRenderer& renderer);
	~TextureManager();

	std::shared_ptr<Texture> LoadTexture(VulkanRenderer& renderer, std::string TextureLocation, VkFormat format);
	std::shared_ptr<CubeMapTexture> LoadTexture(VulkanRenderer& renderer, std::string cubeMapList[6]);
	std::shared_ptr<CubeMapTexture> LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList);
	std::shared_ptr<Texture> LoadTexture(std::shared_ptr<Texture> texture);

	void UnloadTexture(VulkanRenderer& renderer, unsigned int ID);
	void UnloadAllTextures(VulkanRenderer& renderer);
	void UpdateIMGUIVRAM();

	bool GetTextureByName(std::string name);
	bool GetTextureByName(std::string name, std::shared_ptr<Texture>& textureID);
	std::shared_ptr<Texture> GetTextureByID(unsigned int ID);

	std::shared_ptr<Texture> GetTexture(int index) { return TextureList[index]; }
	std::vector<std::shared_ptr<Texture>> GetTextureList() {  return TextureList; }
};

