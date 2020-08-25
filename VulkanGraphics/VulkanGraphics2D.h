#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

#include "RendererManager.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "GUIDebugger.h"
#include "Camera.h"
#include "ModelLoader.h"
#include "Model.h"
#include "SkyBoxMesh.h"
#include "DebugLightMesh.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <chrono>
#include "Texture2D.h"
#include "FrameBufferMesh.h"
#include "GameManager.h"
#include <map>
#include "LevelMesh2D.h"

const std::vector<Vertex> LightVertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
};

const std::vector<uint16_t> LightIndices = {
    0, 1, 2, 2, 3, 0
};

const std::vector<Vertex> MegaManVertices = {
    {{-0.5f, -0.5f, -0.01f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{0.5f, -0.5f,  -0.01f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{0.5f, 0.5f,  -0.01f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
    {{-0.5f, 0.5f,  -0.01f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}}
};

const std::vector<uint16_t> MegaManIndices = {
    0, 1, 2, 2, 3, 0
};

class VulkanGraphics2D
{
private:
    VulkanWindow Window;
    RendererManager renderer;
    GameManager gameManager;

    VulkanRendererSettings CompareVulkanSettings;

    unsigned int LevelBoundsX = 16;
    unsigned int LevelBoundsY = 8;

    std::map<int, glm::ivec2> TileMap;
    std::vector<int> MapLocs;

    std::vector<Vertex> VertexList;
    std::vector<uint16_t> IndexList;

    Keyboard keyboard;
    Mouse mouse;

    Sprite sprite;

    OrthographicCamera orthoCamera;

    LightBufferObject light;

    void UpdateImGUI();
    void Update(uint32_t DrawFrame);
public:
    VulkanGraphics2D(int Width, int Height, const char* AppName);
    ~VulkanGraphics2D();
    void MainLoop();
};
