//#pragma once
//#include "Mesh.h"
//#include <map>
//#include "Animation2D.h"
//
//enum SpriteAnime
//{
//	Stand1,
//	Stand2,
//	StartRun,
//	Run1,
//	Run2,
//	Run3,
//	Run4
//};
//
//struct ColisionGeo
//{
//	std::vector<glm::vec3> CollisionVertexs;
//	std::vector<uint16_t> CollisionIndices;
//};
//
//enum SpriteType
//{
//	SMegaMan,
//	SMMShot
//};
//
//class Sprite
//{
//private:
//	TextureMaps SpriteMaps;
//
//public:
//
//	SpriteType Type;
//	Animation2D CurrentAni;
//
//	std::map<SpriteAnime, glm::vec2> AnimationFrame;
//	Mesh SpriteMesh;
//	glm::vec2 UVOffset = glm::vec2(0.0f);
//	ColisionGeo CollisionVertexs;
//
//	Sprite();
//	Sprite(Renderer& renderer, float Width, float Height, TextureMaps spriteMaps, glm::vec2 StartPos, SpriteType type);
//	Sprite(Renderer& renderer, float Width, float Height, TextureMaps spriteMaps, glm::vec3 StartPos, SpriteType type);
//	~Sprite();
//
//	virtual void Update(Renderer& renderer, Camera& camera, Lights light);
//	void Draw(Renderer& renderer, int currentFrame);
//	void Destory(Renderer& renderer);
//
//	void SetPosition2D(glm::vec2 Pos);
//	void SetPosition2D(float x, float y);
//	void SetPosition3D(glm::vec3 Pos);
//	void SetPosition3D(float x, float y, float z);
//
//	glm::vec2 GetPosition2D() { return glm::vec2(SpriteMesh.MeshPosition.x, SpriteMesh.MeshPosition.y); }
//	glm::vec3 GetPosition3D() { return SpriteMesh.MeshPosition; }
//	float* GetUVOffsetPtr() { return &UVOffset.x; };
//};
//
