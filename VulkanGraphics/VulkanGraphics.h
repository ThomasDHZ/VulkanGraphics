#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "VulkanResources.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_vulkan.h"
#include "ImGui/imgui_impl_glfw.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 6, 7, 4
};

class Testing : VulkanResources
{
private:
	const std::shared_ptr<VulkanRendererInfo> RendererInfo;
public:
	Testing(VulkanRenderer& renderer)
	{
		auto a = GetSwapChainResolution(renderer);
		int  b = 3;
	}
};

class VulkanGraphics
{
private:
	VulkanRendererInfo* RendererInfo;
	VulkanWindow Window;
	VulkanRenderer renderer;

	Texture2D texture;
	std::vector<Mesh> MeshList;

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
