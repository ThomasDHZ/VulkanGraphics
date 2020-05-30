#include "Level2D.h"

Level2D::Level2D()
{
}

Level2D::Level2D(Renderer& renderer, TileSet tileset)
{
	camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f));

	LoadTileSet(renderer, tileset);
	std::vector<Texture2D> textureList = { DiffuseMap, SpecularMap };

	for (int x = 0; x < LevelBoundsX * LevelBoundsY; x++)
	{
		TileList.emplace_back(Mesh(renderer, vertices, indices, textureList));
	}

	glm::vec3 pointLightPositions[] = {
glm::vec3(0.7f,  0.2f,  2.0f),
glm::vec3(2.3f, -3.3f, -4.0f),
glm::vec3(-4.0f,  2.0f, -12.0f),
glm::vec3(0.0f,  0.0f, -3.0f)
	};

	DirectionalLightBuffer lighter;
	lighter.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	lighter.Ambient = glm::vec3(0.0, 0.0, 0.0);
	lighter.Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	lighter.Specular = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLightBuffer light1;
	light1.Position = pointLightPositions[0];
	light1.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light1.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light1.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light1.Constant = 1.0f;
	light1.Linear = 0.09f;
	light1.Quadratic = 0.032;

	PointLightBuffer light2;
	light2.Position = pointLightPositions[1];
	light2.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light2.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light2.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light2.Constant = 1.0f;
	light2.Linear = 0.09f;
	light2.Quadratic = 0.032;

	PointLightBuffer light3;
	light3.Position = pointLightPositions[2];
	light3.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light3.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light3.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light3.Constant = 1.0f;
	light3.Linear = 0.09f;
	light3.Quadratic = 0.032;
	VkPhysicalDeviceRayTracingPropertiesNV AL;

	PointLightBuffer light4;
	light4.Position = pointLightPositions[3];
	light4.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	light4.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light4.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light4.Constant = 1.0f;
	light4.Linear = 0.09f;
	light4.Quadratic = 0.032;

	SpotLightBuffer spotLight = {};
	spotLight.Position = camera.GetCameraPos();
	spotLight.Direction = camera.Front;
	spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	spotLight.Constant = 1.0f;
	spotLight.Linear = 0.09f;
	spotLight.Quadratic = 0.032f;
	spotLight.CutOff = glm::cos(glm::radians(12.5f));
	spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));

	lightManager.DirectionalLightList.emplace_back(DirectionalLight(renderer, lighter));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light1));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light2));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light3));
	lightManager.PointLightList.emplace_back(PointLight(renderer, light4));
	lightManager.SpotlightList.emplace_back(SpotLight(renderer, spotLight));
}

Level2D::~Level2D()
{
}

void Level2D::LevelDebug(Renderer& renderer)
{
	ImGui::Begin("Settings");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("MeshView", &renderer.Settings.ShowMeshLines);
	ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMeshs);
	ImGui::Checkbox("2D Mode", &renderer.Settings.TwoDMode);
	ImGui::SliderFloat3("Camera", camera.GetCameraPosPtr(), -10.0f, 10.0f);
	ImGui::End();

	lightManager.UpdateLights();
}

void Level2D::LevelLoop(Renderer& renderer)
{
}

void Level2D::Update(Renderer& renderer)
{
	Material material = {};
	material.ambient = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
	material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.Shininess = 32.0f;

	MeshProp viewing = {};
	viewing.directionalLight = lightManager.DirectionalLightList[0].GetSettings();
	viewing.pointLight[0] = lightManager.PointLightList[0].GetSettings();
	viewing.pointLight[1] = lightManager.PointLightList[1].GetSettings();
	viewing.pointLight[2] = lightManager.PointLightList[2].GetSettings();
	viewing.pointLight[3] = lightManager.PointLightList[3].GetSettings();
	viewing.spotLight = lightManager.SpotlightList[0].GetSettings();
	viewing.material = material;
	viewing.viewPos = camera.GetCameraPos();


	for (unsigned int x = 0; x < LevelBoundsX; x++)
	{
		for (unsigned int y = 0; y < LevelBoundsY; y++)
		{
			PositionMatrix ubo{};
			ubo.model = glm::mat4(1.0f);
			ubo.model = glm::translate(ubo.model, glm::vec3(x, y, 0.0f));
			ubo.view = camera.GetViewMatrix();
			ubo.proj = glm::perspective(glm::radians(camera.GetCameraZoom()), GetSwapChainResolution(renderer)->width / (float)GetSwapChainResolution(renderer)->height, 0.1f, 100.0f);
			ubo.proj[1][1] *= -1;

			TileList[x + (y * LevelBoundsX)].UpdateUniformBuffer(renderer, ubo, viewing);
		}
	}


	lightManager.Update(renderer, camera);
}

void Level2D::Draw(Renderer& renderer, uint32_t DrawFrame)
{
	for (auto tile : TileList)
	{
		tile.Draw(renderer, DrawFrame);
	}
	if (renderer.Settings.ShowDebugLightMeshs)
	{
		lightManager.DrawDebugMesh(renderer, DrawFrame);
	}
}

void Level2D::LoadTileSet(Renderer& renderer, TileSet tileset)
{
	DiffuseMap = Texture2D(renderer, tileset.DiffuseMap);
	SpecularMap = Texture2D(renderer, tileset.SpecularMap);
}

void Level2D::UnloadTileSet(Renderer& renderer)
{
	DiffuseMap.Destroy(renderer);
	SpecularMap.Destroy(renderer);
	NormalMap.Destroy(renderer);
}

void Level2D::Destroy(Renderer& renderer)
{
	UnloadTileSet(renderer);
	lightManager.Destroy(renderer);
	for (auto tile : TileList)
	{
		tile.Destroy(renderer);
	}
}
