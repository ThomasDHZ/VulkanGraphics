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
#include "LevelMesh2D.h"


const std::vector<Vertex> vertices =
{
      {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
      {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
      {{-1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
      {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} }
};

const std::vector<uint16_t> indices =
{
};

const std::vector<Vertex> quadvertices = {
        {{-0.5,-0.5,-0.5}, {0,-1,0}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,-0.5}, {0,-1,0}, {1,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,0.5}, {0,-1,0}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,-0.5,0.5}, {0,-1,0}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,-0.5}, {0,0,-1}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,-0.5}, {0,0,-1}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,-0.5}, {0,0,-1}, {1,1}, {0,-1,0}, {0,-1,0}},
        {{-0.5,-0.5,-0.5}, {0,0,-1}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,-0.5}, {1,0,0}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,0.5}, {1,0,0}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,0.5}, {1,0,0}, {1,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,-0.5}, {1,0,0}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,0.5}, {0,0,1}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,0.5}, {0,0,1}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,-0.5,0.5}, {0,0,1}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{0.5,-0.5,0.5}, {0,0,1}, {1,1}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,0.5}, {-1,0,0}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,-0.5}, {-1,0,0}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{-0.5,-0.5,-0.5}, {-1,0,0}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{-0.5,-0.5,0.5}, {-1,0,0}, {1,1}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,-0.5}, {0,1,0}, {0,1}, {0,-1,0}, {0,-1,0}},
        {{-0.5,0.5,0.5}, {0,1,0}, {0,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,0.5}, {0,1,0}, {1,0}, {0,-1,0}, {0,-1,0}},
        {{0.5,0.5,-0.5}, {0,1,0}, {1,1}, {0,-1,0}, {0,-1,0}},
};

const std::vector<uint16_t> quadindices = {
        0,1,2, 0,2,3,
        4,5,6, 4,6,7,
        8,9,10, 8,10,11,
        12,13,14, 12,14,15,
        16,17,18, 16,18,19,
        20,21,22, 20,22,23,
};

class VulkanGraphics
{
private:
    VulkanWindow Window;
    RendererManager renderer;
    GameManager gameManager;

    Keyboard keyboard;
    Mouse mouse;


	ModelLoader modelLoader;

	VulkanRendererSettings CompareVulkanSettings;

    std::vector<Texture> TextureList;

    Camera* ActiveCamera;

    LightBufferObject light;

	//std::vector<Model> ModelList;


    CubeMapLayout layout;

    unsigned int LevelBoundsX = 16;
    unsigned int LevelBoundsY = 8;

    std::map<int, glm::ivec2> TileMap;
    std::vector<int> MapLocs;

    std::vector<Vertex> VertexList;
    std::vector<uint16_t> IndexList;


    void UpdateImGUI();
	void Update(uint32_t DrawFrame);
    void LoadTiles(std::shared_ptr<TextureManager> textureManager);
    void CreateLevelGeometry(std::shared_ptr<TextureManager> textureManager);
    std::vector<Vertex> CalcVertex();
public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
