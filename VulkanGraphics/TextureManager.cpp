#include "TextureManager.h"

TextureManager::TextureManager()
{
}

TextureManager::TextureManager(VulkanRenderer& renderer)
{
}

TextureManager::~TextureManager()
{
}

unsigned int TextureManager::CreateNewTextureID()
{
	return IDNum++;
}

void TextureManager::LoadTexture(VulkanRenderer& renderer, std::string TextureLocation)
{
	TextureList.emplace_back(Texture2D(renderer, TextureLocation, CreateNewTextureID()));
}

void TextureManager::LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList)
{
	TextureList.emplace_back(CubeMapTexture(renderer, cubeMapList, CreateNewTextureID()));
}

void TextureManager::LoadTexture(Texture texture)
{
	texture.TextureID = CreateNewTextureID();
	TextureList.emplace_back(texture);
}

void TextureManager::UnloadTexture(VulkanRenderer& renderer, unsigned int ID)
{
	auto texture = GetTextureByID(ID);
	texture.Delete(renderer);
	//TextureList.erase(texture);
}

void TextureManager::UnloadAllTextures(VulkanRenderer& renderer)
{
	for (auto& texture : TextureList)
	{
		if (texture.TypeOfTexture != TextureType::vkRenderedTexture)
		{
			//Rendered textures gets deleted with the renderer;
			texture.Delete(renderer);
		}
	}
	TextureList.erase(TextureList.begin(), TextureList.end());
}

void TextureManager::UpdateIMGUIVRAM()
{
	ImGui::Begin("VRAM");
	for (int x = 0; x < GetTextureList().size(); x++)
	{
		if (TextureList[x].TypeOfTexture != TextureType::vkTextureCube)
		{
			ImGui::Image(TextureList[x].ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
		}
	}
	ImGui::End();
}

bool TextureManager::GetTextureByName(std::string name)
{
	for (auto texture : TextureList)
	{
		if (texture.FileName == name)
		{
			return true;
		}
	}
	
	return false;
}

Texture TextureManager::GetTextureByID(unsigned int ID)
{
	for (auto texture : TextureList)
	{
		if (texture.TextureID == ID)
		{
			return texture;
		}
	}

	std::cout << "Texture ID: " << ID << "not found." << std::endl;
	return Texture();
}
