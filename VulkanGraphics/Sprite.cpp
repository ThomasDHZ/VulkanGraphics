#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(Renderer& renderer) : Mesh(renderer)
{
	const std::vector<Vertex> MegaManVertices = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		{{0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
	{ {0.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  1.0f }},
	{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  1.0f }},

	{ {0.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { 0.0f,  0.0f }},
	{ {1.0f, -0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  1.0f }},
	{ {1.0f,  0.5f,  0.0f }, { 0.0f,  0.0f, -1.0f }, { .11f,  0.0f }}
	};

	const std::vector<uint16_t> indices = {

	};



	IndiceSize = 0;

	TextureMaps maps;
	maps.DiffuseMap = Texture2D(renderer, "texture/MegaManDiffuse2048.bmp");
	maps.SpecularMap = Texture2D(renderer, "texture/MegaManSpecular2048.bmp");

	VertexList = MegaManVertices;
	//IndexList = indices;
	TextureList = maps;

	CreateVertexBuffer(renderer);
	//CreateIndiceBuffer(renderer);
	CreateUniformBuffers(renderer);
	CreateDescriptorPool(renderer);
	CreateDescriptorSets(renderer);
}


Sprite::~Sprite()
{
}

void Sprite::UpdateSpriteUVs(Renderer& renderer)
{
	CreateVertexBuffer(renderer);
	CreateIndiceBuffer(renderer);

}