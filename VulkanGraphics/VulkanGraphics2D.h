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
#include "SkyBox.h"

//const std::vector<Vertex> vertices =
//{
//	{{-0.5,-0.5,-0.5}, {0,-1,0}, {0,1}},
//	{{0.5,-0.5,-0.5}, {0,-1,0}, {1,1}},
//	{{0.5,-0.5,0.5}, {0,-1,0}, {1,0}},
//	{{-0.5,-0.5,0.5}, {0,-1,0}, {0,0}},
//	{{-0.5,0.5,-0.5}, {0,0,-1}, {0,0}},
//	{{0.5,0.5,-0.5}, {0,0,-1}, {1,0}},
//	{{0.5,-0.5,-0.5}, {0,0,-1}, {1,1}},
//	{{-0.5,-0.5,-0.5}, {0,0,-1}, {0,1}},
//	{{0.5,0.5,-0.5}, {1,0,0}, {0,0}},
//	{{0.5,0.5,0.5}, {1,0,0}, {1,0}},
//	{{0.5,-0.5,0.5}, {1,0,0}, {1,1}},
//	{{0.5,-0.5,-0.5}, {1,0,0}, {0,1}},
//	{{0.5,0.5,0.5}, {0,0,1}, {1,0}},
//	{{-0.5,0.5,0.5}, {0,0,1}, {0,0}},
//	{{-0.5,-0.5,0.5}, {0,0,1}, {0,1}},
//	{{0.5,-0.5,0.5}, {0,0,1}, {1,1}},
//	{{-0.5,0.5,0.5}, {-1,0,0}, {1,0}},
//	{{-0.5,0.5,-0.5}, {-1,0,0}, {0,0}},
//	{{-0.5,-0.5,-0.5}, {-1,0,0}, {0,1}},
//	{{-0.5,-0.5,0.5}, {-1,0,0}, {1,1}},
//	{{-0.5,0.5,-0.5}, {0,1,0}, {0,1}},
//	{{-0.5,0.5,0.5}, {0,1,0}, {0,0}},
//	{{0.5,0.5,0.5}, {0,1,0}, {1,0}},
//	{{0.5,0.5,-0.5}, {0,1,0}, {1,1}},
//};
//
//const std::vector<uint16_t> indices = {
//	0,1,2, 0,2,3,
//	4,5,6, 4,6,7,
//	8,9,10, 8,10,11,
//	12,13,14, 12,14,15,
//	16,17,18, 16,18,19,
//	20,21,22, 20,22,23,
//};

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
    FrameBufferMesh framebuffer1;
    FrameBufferMesh framebuffer2;
    FrameBufferMesh framebuffer3;

    Mesh mesh;

    std::shared_ptr<PerspectiveCamera> PCamera;
    std::shared_ptr<OrthographicCamera> OrthoCamera;
    OrthographicCamera OrthoCamera2;

    Light light;
    SkyBox skybox;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    void UpdateImGUI();
    void Update(uint32_t DrawFrame, std::shared_ptr<Camera> camera);
public:
    VulkanGraphics2D(int Width, int Height, const char* AppName);
    ~VulkanGraphics2D();
    void MainLoop();
};
