#pragma once
#include <vulkan\vulkan_core.h>
#include <array>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <array>
#include <optional>
#include <set>
#include "Texture.h"
#include <future>
#include <thread>
#include <chrono>
#include <iostream>

class Texture2D : public Texture
{
private:
	void CreateTextureSampler();
public:
	std::vector<Pixel> PixelImage;
	std::vector<Pixel> textureBytes;

	Texture2D();
	Texture2D(VulkanDevice deviceInfo, std::string TexturePath);
	Texture2D(VulkanDevice deviceInfo, int width, int height, Pixel TextureColor);
	~Texture2D();

	void UpdateTexture(Pixel pixel);

};

