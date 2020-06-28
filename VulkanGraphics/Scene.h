#pragma once
#include "Camera.h"
#include "LightManager.h"
#include "Skybox.h"
#include "Model.h"
class Scene
{
private:
	Camera camera;
	LightManager lightManager;

	SkyBox Skybox;
	std::vector<Mesh> MeshList;
	std::vector<Model> ModelList;
public:
};

