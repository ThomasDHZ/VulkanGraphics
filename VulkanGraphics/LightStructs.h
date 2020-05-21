#pragma once
#include "VulkanResources.h"

struct DirectionalLightBuffer
{
	alignas(16) glm::vec3 Direction;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;
};

struct PointLightBuffer
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;

	alignas(4) float Constant;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
};

struct SpotLightBuffer
{
	alignas(16) glm::vec3 Position;
	alignas(16) glm::vec3 Direction;
	alignas(16) glm::vec3 Ambient;
	alignas(16) glm::vec3 Diffuse;
	alignas(16) glm::vec3 Specular;

	alignas(4) float CutOff;
	alignas(4) float OuterCutOff;
	alignas(4) float Constant;
	alignas(4) float Linear;
	alignas(4) float Quadratic;
};