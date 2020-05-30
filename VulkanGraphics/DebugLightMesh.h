#pragma once
#include "BaseMesh.h"
#include "UniformBuffer.h"
#include "Mesh.h"

class DebugLightMesh : public BaseMesh
{
private:

protected:
	void CreateUniformBuffers(Renderer& renderer);
	void CreateDescriptorPool(Renderer& renderer);
	void CreateDescriptorSets(Renderer& renderer);

public:

	UniformBuffer PositionMatrixBuffer;

	DebugLightMesh();
	DebugLightMesh(Renderer& renderer, const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
	DebugLightMesh(Renderer& renderer, const std::vector<Vertex>& vertices);
	~DebugLightMesh();

	void Draw(Renderer& renderer, int currentFrame);
	void UpdateUniformBuffer(Renderer& renderer, PositionMatrix positionMatrix);
	void Destroy(Renderer& renderer);
};

