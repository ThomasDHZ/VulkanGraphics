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

unsigned int TextureManager::LoadTexture(VulkanRenderer& renderer, std::string TextureLocation, VkFormat format)
{
	unsigned int TextureID;
	if (!GetTextureByName(TextureLocation, TextureID))
	{
		TextureID = CreateNewTextureID();
		auto a = std::make_shared<Texture2D>(Texture2D(renderer, format, TextureLocation, TextureID));
		TextureList.emplace_back(a);
	}

	return TextureID;
}

unsigned int TextureManager::LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList)
{
	unsigned int TextureID = CreateNewTextureID();
	auto a = std::make_shared<CubeMapTexture>(CubeMapTexture(renderer, cubeMapList, TextureID));
	TextureList.emplace_back(a);
	return TextureID;
}

unsigned int TextureManager::LoadTexture(std::shared_ptr<Texture> texture)
{
	unsigned int TextureID = CreateNewTextureID();
	texture->TextureID = TextureID;
	TextureList.emplace_back(texture);

	return TextureID;
}

void TextureManager::UnloadTexture(VulkanRenderer& renderer, unsigned int ID)
{
	auto texture = GetTextureByID(ID);
	texture->Delete(renderer);
	//TextureList.erase(texture);
}

void TextureManager::UnloadAllTextures(VulkanRenderer& renderer)
{
	for (auto& texture : TextureList)
	{
		if (texture->TypeOfTexture != TextureType::vkRenderedTexture)
		{
			//Rendered textures gets deleted with the renderer;
			texture->Delete(renderer);
		}
	}
	TextureList.erase(TextureList.begin(), TextureList.end());
}

void TextureManager::UpdateIMGUIVRAM()
{
	ImGui::Begin("VRAM");
	for (int x = 0; x < GetTextureList().size(); x++)
	{
		if (TextureList[x]->TypeOfTexture != TextureType::vkTextureCube)
		{
			ImGui::Image(TextureList[x]->ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
		}
	}
	ImGui::End();
}

bool TextureManager::GetTextureByName(std::string name)
{
	for (auto texture : TextureList)
	{
		if (texture->FileName == name)
		{
			return true;
		}
	}
	
	return false;
}

bool TextureManager::GetTextureByName(std::string name, unsigned int& textureID)
{
	for (auto texture : TextureList)
	{
		if (texture->FileName == name)
		{
			textureID = texture->TextureID;
			return true;
		}
	}

	return false;
}

std::shared_ptr<Texture> TextureManager::GetTextureByID(unsigned int ID)
{
	for (auto texture : TextureList)
	{
		if (texture->TextureID == ID)
		{
			return texture;
		}
	}

	std::cout << "Texture ID: " << ID << "not found." << std::endl;
	return std::shared_ptr<Texture>();
}
