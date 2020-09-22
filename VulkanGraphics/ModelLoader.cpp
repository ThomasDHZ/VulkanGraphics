#include "ModelLoader.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::ModelLoader(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath)
{
	LoadModel(renderer, textureManager, FilePath);
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::LoadModel(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath)
{
	Assimp::Importer ModelImporter;

	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
	{
		std::cout << "Error loading model: " << ModelImporter.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(renderer, textureManager, FilePath, Scene->mRootNode, Scene);
}

void ModelLoader::ProcessNode(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		SubMesh NewMesh;

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		NewMesh.VertexList = LoadVertices(mesh);
		NewMesh.IndexList = LoadIndices(mesh);
		NewMesh.TextureList = LoadTextures(renderer, textureManager, FilePath, mesh, scene);
		
		ModelMeshList.emplace_back(NewMesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(renderer, textureManager, FilePath, node->mChildren[i], scene);
	}
}

std::vector<Vertex> ModelLoader::LoadVertices(aiMesh* mesh)
{
	std::vector<Vertex> VertexList;

	for (int x = 0; x < mesh->mNumVertices; x++)
	{
		Vertex vertex;
		vertex.Position = glm::vec3{ mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z };
		vertex.Normal = glm::vec3{ mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z };

		if (mesh->mTangents)
		{
			vertex.Tangant = glm::vec3{ mesh->mTangents[x].x, mesh->mTangents[x].y, mesh->mTangents[x].z };
			vertex.BiTangant = glm::vec3{ mesh->mBitangents[x].x, mesh->mBitangents[x].y, mesh->mBitangents[x].z };
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexureCoord = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
		}
		else
		{
			vertex.TexureCoord = glm::vec2{ 0.0f, 0.0f };
		}

		VertexList.emplace_back(vertex);
	}

	return VertexList;
}

std::vector<uint16_t> ModelLoader::LoadIndices(aiMesh* mesh)
{
	std::vector<uint16_t> IndexList;

	for (int x = 0; x < mesh->mNumFaces; x++)
	{
		aiFace Faces = mesh->mFaces[x];
		for (int y = 0; y < Faces.mNumIndices; y++)
		{
			IndexList.emplace_back(Faces.mIndices[y]);
		}
	}

	return IndexList;
}

std::vector<Texture2D> ModelLoader::LoadTextures(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiMesh* mesh, const aiScene* scene)
{
	std::vector<Texture2D> TextureList;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto directory = FilePath.substr(0, FilePath.find_last_of('/')) + '/';

	auto a = material->GetTextureCount(aiTextureType_DIFFUSE);
	auto b = material->GetTextureCount(aiTextureType_SPECULAR);
	auto c = material->GetTextureCount(aiTextureType_HEIGHT);
	auto d = material->GetTextureCount(aiTextureType_AMBIENT);
	auto e = material->GetTextureCount(aiTextureType_OPACITY);

	for (int x = 0; x <= aiTextureType_UNKNOWN; x++)
	{
		for (unsigned int y = 0; y < material->GetTextureCount(static_cast<aiTextureType>(x)); y++)
		{
			aiString TextureLocation;
			material->GetTexture(static_cast<aiTextureType>(x), y, &TextureLocation);
			material->GetTexture(aiTextureType_NORMALS, y, &TextureLocation);
			if(!textureManager->GetTextureByName(directory + TextureLocation.C_Str()))
			{
				textureManager->LoadTexture(renderer, directory + TextureLocation.C_Str(), VK_FORMAT_R8G8B8A8_UNORM);
			}
		}
	}
	return TextureList;
}
