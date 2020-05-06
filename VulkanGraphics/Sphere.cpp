//
//#include "Sphere.h"
//
//Sphere::Sphere()
//{
//}
//
//Sphere::Sphere(VulkanRenderer& Renderer, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, const std::vector<Texture2D>& textureList) : Mesh(Renderer, textureList)
//{
//	MeshPosition = Position;
//	MeshRotate = Rotation;
//	MeshScale = Scale;
//
//	std::vector<glm::vec3> positions;
//	std::vector<glm::vec2> uv;
//	std::vector<glm::vec3> normals;
//
//	const unsigned int X_SEGMENTS = 6;
//	const unsigned int Y_SEGMENTS = 6;
//	const float PI = 3.14159265359;
//	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
//	{
//		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
//		{
//			float xSegment = (float)x / (float)X_SEGMENTS;
//			float ySegment = (float)y / (float)Y_SEGMENTS;
//			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//			float yPos = std::cos(ySegment * PI);
//			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
//
//			positions.push_back(glm::vec3(xPos, yPos, zPos));
//			uv.push_back(glm::vec2(xSegment, ySegment));
//			normals.push_back(glm::vec3(xPos, yPos, zPos));
//		}
//	}
//
//	bool oddRow = false;
//	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
//	{
//		if (!oddRow)
//		{
//			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
//			{
//				IndexList.push_back(y * (X_SEGMENTS + 1) + x);
//				IndexList.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//			}
//		}
//		else
//		{
//			for (int x = X_SEGMENTS; x >= 0; --x)
//			{
//				IndexList.push_back((y + 1) * (X_SEGMENTS + 1) + x);
//				IndexList.push_back(y * (X_SEGMENTS + 1) + x);
//			}
//		}
//		oddRow = !oddRow;
//	}
//
//	for (std::size_t i = 0; i < positions.size(); ++i)
//	{
//		Vertex vertex;
//		vertex.Position = glm::vec3(positions[i].x, positions[i].y, positions[i].z);
//		if (uv.size() > 0)
//		{
//			vertex.TexureCoord = glm::vec2(uv[i].x, uv[i].y);
//		}
//		if (normals.size() > 0)
//		{
//			vertex.TexureCoord = glm::vec3(normals[i].x, normals[i].y, normals[i].z);
//		}
//
//		VertexList.emplace_back(vertex);
//	}
//
//	VertexSize = VertexList.size();
//	IndiceSize = IndexList.size();
//
//	CreateVertexBuffer(Renderer);
//	CreateIndiceBuffer(Renderer);
//	CreateUniformBuffers(Renderer);
//	CreateDescriptorPool(Renderer);
//	CreateDescriptorSets(Renderer);
//}
//
//Sphere::~Sphere()
//{
//}
