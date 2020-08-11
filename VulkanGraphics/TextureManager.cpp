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

void TextureManager::LoadTexture(VulkanRenderer& renderer, std::string TextureLocation)
{
	TextureList.emplace_back(Texture2D(renderer, TextureLocation));
}

void TextureManager::LoadTexture(VulkanRenderer& renderer, CubeMapLayout cubeMapList)
{
	CubeMapTextureList.emplace_back(CubeMapTexture(renderer, cubeMapList));
}

void TextureManager::LoadTexture(Texture texture)
{
	TextureList.emplace_back(texture);
}

void TextureManager::UnloadTexture(VulkanRenderer& renderer)
{
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
		ImGui::Image(TextureList[x].ImGuiDescriptorSet, ImVec2(80.0f, 80.0f));
	}
	ImGui::End();
}
