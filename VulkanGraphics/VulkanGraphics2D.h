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
#include "Mesh2D.h"
#include "Light.h"
#include "Sprite.h"
#include "Level2D.h"
#include "LevelSprite.h"

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

    Keyboard keyboard;
    Mouse mouse;
    
    std::vector<std::shared_ptr<Object2D>> SpriteList;
 
    OrthographicCamera OrthoCamera;
    OrthographicCamera OrthoCamera2;

    Light light;

    NewMesh mesh;

    void UpdateImGUI();
    void Update(uint32_t DrawFrame, OrthographicCamera& camera);
public:
    VulkanGraphics2D(int Width, int Height, const char* AppName);
    ~VulkanGraphics2D();
    void MainLoop();
};
