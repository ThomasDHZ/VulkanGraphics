#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>

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
#include "ForwardRenderingPipeline.h"
#include "Vertex.h"
#include "VulkanSwapChain.h"
#include "InputAttachment.h"
#include "VulkanRenderer.h"
#include "Texture2D.h"
#include "Mesh.h"
#include "VulkanWindow.h"
#include "GUIDebugger.h"
#include "Skybox.h"
#include "Camera.h"

const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
	{{0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		 {{0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f,  0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},
		{{-0.5f, -0.5f, -0.5f},  {0.0f,  0.0f, -1.0f}},

		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		 {{0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f,  0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},
		{{-0.5f, -0.5f,  0.5f},  {0.0f,  0.0f,  1.0f}},

		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {-1.0f,  0.0f,  0.0f}},

		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {1.0f,  0.0f,  0.0f}},

		{{-0.5f, -0.5f, -0.5f},  {0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f, -0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		 {{0.5f, -0.5f,  0.5f},  { 0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f,  0.5f},  {  0.0f, -1.0f,  0.0f}},
		{{-0.5f, -0.5f, -0.5f},  {  0.0f, -1.0f,  0.0f}},

		{{-0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		{{ 0.5f,  0.5f, -0.5f},  { 0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		 {{0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f,  0.5f},  {  0.0f,  1.0f,  0.0f}},
		{{-0.5f,  0.5f, -0.5f},  {  0.0f,  1.0f,  0.0f}}
};

const std::vector<uint16_t> indices = {

};

class VulkanGraphics2
{
private:
	Camera camera;
	VulkanSettings CompareVulkanSettings;
	VulkanRenderer Renderer;
	GUIDebugger guiDebugger;

	VulkanWindow Window;
	Lighter light;

	Texture2D texture;
	Mesh mesh;

	SkyBox Skybox;
	CubeMapTexture SkyboxTexture;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX;
	float lastY;
	bool firstMouse;
	double MouseXPos;
	double MouseYPos;

	bool framebufferResized = false;

	void InitializeGUIDebugger();
	void mainLoop();
	void updateUniformBuffer(uint32_t currentImage);
	void UpdateCommandBuffers(uint32_t NextFrameIndex);
	void Draw();
	void UpdateMouse();
	void UpdateKeyboard();

public:
	VulkanGraphics2(int Width, int Height, const char* AppName);
	~VulkanGraphics2();

	void run() 
	{
		mainLoop();
	}
};

