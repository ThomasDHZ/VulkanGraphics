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
#include "Keyboard.h"
#include "Mouse.h"
#include <chrono>
#include "Texture2D.h"
#include "FrameBufferMesh.h"
#include "GameManager.h"
#include "Object.h"
#include "Light.h"
#include "SkyBox.h"

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



class VulkanGraphics
{
private:
    VulkanWindow Window;
    RendererManager renderer;
    GameManager gameManager;

	ModelLoader modelLoader;

	VulkanRendererSettings CompareVulkanSettings;

    std::vector<Texture> TextureList;

	Keyboard keyboard;
	Mouse mouse;

    PerspectiveCamera* ActiveCamera;


  
    std::vector<Object> obj;
    Light light;
    SkyBox skybox;
	//std::vector<Model> ModelList;

    void UpdateImGUI();
	void Update(uint32_t DrawFrame);
    std::vector<Vertex> CalcVertex();
public:
	VulkanGraphics(int Width, int Height, const char* AppName);
	~VulkanGraphics();
	void MainLoop();
};
