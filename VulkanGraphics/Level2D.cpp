#include "Level2D.h"
#include "VulkanGraphics.h"

Level2D::Level2D()
{
}

Level2D::Level2D(Renderer& renderer, TileSet tileset)
{
	camera = Camera(glm::vec3(3.2f, 10.0f, 10.0f));
	LevelMap = LevelMesh2D(renderer, tileset);

	CubeMapLayout layout;
	layout.Left = "texture/skybox/left.jpg";
	layout.Right = "texture/skybox/right.jpg";
	layout.Top = "texture/skybox/top.jpg";
	layout.Bottom = "texture/skybox/bottom.jpg";
	layout.Back = "texture/skybox/back.jpg";
	layout.Front = "texture/skybox/front.jpg";

	TextureMaps maps = {};
	maps.DiffuseMap = Texture2D(renderer, "texture/MegaManDiffuse.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/MegaManSpecular.bmp");
	maps.NormalMap = Texture2D(renderer, "texture/MegaManSpecular.bmp");
	maps.DisplacementMap = Texture2D(renderer, "texture/MegaManSpecular.bmp");
	maps.AlphaMap = Texture2D(renderer, "texture/MegaManAlpha.bmp");
	maps.EmissionMap = Texture2D(renderer, "texture/temp.bmp");
	maps.CubeMap = CubeMapTexture(renderer, layout);

	TextureMaps maps2 = {};
	maps2.DiffuseMap = Texture2D(renderer, "texture/Shoot.png");
	maps2.SpecularMap = Texture2D(renderer, "texture/temp.bmp");
	maps2.NormalMap = Texture2D(renderer, "texture/temp.bmp");
	maps2.DisplacementMap = Texture2D(renderer, "texture/temp.bmp");
	maps2.AlphaMap = Texture2D(renderer, "texture/temp.bmp");
	maps2.EmissionMap = Texture2D(renderer, "texture/Shoot.png");
	maps2.CubeMap = CubeMapTexture(renderer, layout);

	SpriteList.emplace_back(MegaMan(renderer, maps, glm::vec2(3.33f, 8.0f)));
	SpriteList.emplace_back(MMShot(renderer, maps2, glm::vec3(3.33f, 8.38f, 0.0)));

	light.directionalLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	light.directionalLight.Ambient = glm::vec3(0.005f, 0.005f, 0.005f);
	light.directionalLight.Diffuse = glm::vec3(0.04f, 0.04f, 0.04f);
	light.directionalLight.Specular = glm::vec3(0.05f, 0.05f, 0.05f);
	light.pointLight.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	light.pointLight.Ambient = glm::vec3(0.005f);
	light.pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	light.pointLight.Specular = glm::vec3(0.3f);
	light.pointLight.Constant = 1.0f;
	light.pointLight.Linear = 0.09f;
	light.pointLight.Quadratic = 0.032f;
	light.spotLight.Ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	light.spotLight.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
	light.spotLight.Constant = 1.0f;
	light.spotLight.Linear = 0.09f;
	light.spotLight.Quadratic = 0.032f;
	light.spotLight.CutOff = glm::cos(glm::radians(12.5f));
	light.spotLight.OuterCutOff = glm::cos(glm::radians(15.0f));
}

Level2D::~Level2D()
{
}

void Level2D::LevelDebug(Renderer& renderer)
{
	ImGui::Begin("Settings");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Checkbox("Mesh View", &renderer.Settings.ShowMeshLines);
	ImGui::Checkbox("Collision View", &renderer.Settings.ShowDebugCollisionMesh);
	ImGui::Checkbox("Show Light Debug Meshes", &renderer.Settings.ShowDebugLightMesh);
	ImGui::Checkbox("2D Mode", &renderer.Settings.TwoDMode);
	ImGui::SliderFloat3("Camera", &camera.Position.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Sprite", SpriteList[0].SpriteMesh.GetMeshPosPtr(), -10.0f, 10.0f);
	ImGui::SliderFloat2("UV", &SpriteList[0].SpriteMesh.properites.SpriteUV.x, .7, 1.0f);
	ImGui::SliderInt("Flip", &SpriteList[0].SpriteMesh.properites.FlipTexture, 0.0f, 1.0f);
	ImGui::SliderFloat3("Sprite2", SpriteList[1].SpriteMesh.GetMeshPosPtr(), 8.0f, 10.0f);
	ImGui::SliderFloat2("UV2", &SpriteList[1].SpriteMesh.properites.SpriteUV.x, .7, 1.0f);
	ImGui::SliderInt("Flip2", &SpriteList[1].SpriteMesh.properites.FlipTexture, 0.0f, 1.0f);
	ImGui::End();
}

void Level2D::LevelLoop(Renderer& renderer)
{
}

void Level2D::Update(Renderer& renderer, GLFWwindow* Window)
{
	light.spotLight.Position = camera.Position;
	light.spotLight.Direction = camera.Front;
	light.viewPos = camera.Position;

	LevelMap.Update(renderer, camera, light);
	for (auto sprite : SpriteList)
	{
		switch (sprite.Type)
		{
			case SpriteType::SMegaMan:
			{
				 static_cast<MegaMan*>(&sprite)->Update(Window, renderer, camera, light); break;
			}
			case SpriteType::SMMShot: static_cast<MMShot*>(&sprite)->Update(Window, renderer, camera, light); break;
		}
	}
}

void Level2D::Draw(Renderer& renderer, uint32_t DrawFrame)
{
	LevelMap.Draw(renderer, DrawFrame);
	for (auto& sprite : SpriteList)
	{
		sprite.Draw(renderer, DrawFrame);
	}
}

void Level2D::Destroy(Renderer& renderer)
{
	LevelMap.Destory(renderer);
	for (auto& sprite : SpriteList)
	{
		sprite.Destory(renderer);
	}
}
