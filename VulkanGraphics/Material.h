#pragma once
#include "Texture2D.h"
#include "CubeMapTexture.h"
#include "LightStructs.h"

struct TextureMaterial
{
	Texture2D DiffuseMap;
	Texture2D SpecularMap;
	Texture2D NormalMap;
	Texture2D DisplacementMap;
	Texture2D AlphaMap;
	CubeMapTexture CubeMap;
};

struct Materialz
{
	alignas(16) glm::vec3 ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
	alignas(4)  float Shininess;
};

struct MeshProp
{
	DirectionalLightBuffer directionalLight;
	PointLightBuffer pointLight;
	SpotLightBuffer spotLight;
	Materialz material;
	alignas(16) glm::vec3 viewPos;
	alignas(8) glm::vec2 SpriteUV;
};

class Material
{
private:
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);

public:
};



