#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "VulkanResources.h"
#include "GUIDebugger.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


//const std::vector<Vertex> vertices =
//{
//	{{-0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
//	{{0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
//	{{-0.5f,0.5f,-0.5f}, { 0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
//	{{0.5f,0.5f,-0.5f}, { 0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
//
//	{{-0.5f,-0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
//	{{0.5f,-0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
//	{{-0.5f,0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//	{{0.5f,0.5f,0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
//
//	{{-0.5f,-0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//	{{-0.5f,0.5f,-0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//	{{-0.5f,-0.5f,0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//	{{-0.5f,0.5f,0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//
//	{{0.5f,-0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
//	{{0.5f,0.5f,-0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//	{{0.5f,-0.5f,0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
//	{{0.5f,0.5f,0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
//
//	{{-0.5f,-0.5f,-0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
//	{{0.5f,-0.5f,-0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
//	{{-0.5f,-0.5f,0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
//	{{0.5f,-0.5f,0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
//
//	{{-0.5f,0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//	{{0.5f,0.5f,-0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
//	{{-0.5f,0.5f,0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
//	{{0.5f,0.5f,0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
//};
//
//const std::vector<uint16_t> indices = {
//	0,2, 1,    2,3,1,
//	4,5, 7,    4,7,6,
//	8,10, 9,  10,11,9,
//	12,13,15, 12,15,14,
//	16,17,18, 18,17,19,
//	20,23,21, 20,22,23
//};

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

class VulkanGraphics : VulkanResources
{
private:
	VulkanWindow Window;
	VulkanRenderer renderer;

	Texture2D texture;
	std::vector<Mesh> MeshList;
	GUIDebugger guiDebugger;

	void InitializeGUIDebugger();
	void Update(uint32_t NextFrameIndex);
	void UpdateCommandBuffers(uint32_t NextFrameIndex);
	void Draw();

public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
