//#include "ModelLoader.h"
//
//ModelLoader::ModelLoader()
//{
//}
//
//ModelLoader::ModelLoader(Renderer& renderer, const std::string& FilePath)
//{
//	LoadModel(renderer, FilePath);
//}
//
//ModelLoader::~ModelLoader()
//{
//}
//
//void ModelLoader::LoadModel(Renderer& renderer, const std::string& FilePath)
//{
//	Assimp::Importer ModelImporter;
//
//	const aiScene* Scene = ModelImporter.ReadFile(FilePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//	if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
//	{
//		std::cout << "Error loading model: " << ModelImporter.GetErrorString() << std::endl;
//		return;
//	}
//
//	ProcessNode(renderer, FilePath, Scene->mRootNode, Scene);
//}
//
//void ModelLoader::ProcessNode(Renderer& renderer, const std::string& FilePath, aiNode* node, const aiScene* scene)
//{
//	for (unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		SubMesh NewMesh;
//
//		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//		NewMesh.VertexList = LoadVertices(mesh);
//		NewMesh.IndexList = LoadIndices(mesh);
//		NewMesh.TextureList = LoadTextures(renderer, FilePath, mesh, scene);
//		
//		ModelMeshList.emplace_back(NewMesh);
//	}
//
//	for (unsigned int i = 0; i < node->mNumChildren; i++)
//	{
//		ProcessNode(renderer, FilePath, node->mChildren[i], scene);
//	}
//}
//
//std::vector<Vertex> ModelLoader::LoadVertices(aiMesh* mesh)
//{
//	std::vector<Vertex> VertexList;
//
//	for (int x = 0; x < mesh->mNumVertices; x++)
//	{
//		Vertex vertex;
//		vertex.Position = glm::vec3{ mesh->mVertices[x].x, mesh->mVertices[x].y, mesh->mVertices[x].z };
//		vertex.Normal = glm::vec3{ mesh->mNormals[x].x, mesh->mNormals[x].y, mesh->mNormals[x].z };
//
//		if (mesh->mTangents)
//		{
//			vertex.Tangant = glm::vec3{ mesh->mTangents[x].x, mesh->mTangents[x].y, mesh->mTangents[x].z };
//			vertex.BiTangant = glm::vec3{ mesh->mBitangents[x].x, mesh->mBitangents[x].y, mesh->mBitangents[x].z };
//		}
//
//		if (mesh->mTextureCoords[0])
//		{
//			vertex.TexureCoord = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
//		}
//		else
//		{
//			vertex.TexureCoord = glm::vec2{ 0.0f, 0.0f };
//		}
//
//		VertexList.emplace_back(vertex);
//	}
//
//	return VertexList;
//}
//
//std::vector<uint16_t> ModelLoader::LoadIndices(aiMesh* mesh)
//{
//	std::vector<uint16_t> IndexList;
//
//	for (int x = 0; x < mesh->mNumFaces; x++)
//	{
//		aiFace Faces = mesh->mFaces[x];
//		for (int y = 0; y < Faces.mNumIndices; y++)
//		{
//			IndexList.emplace_back(Faces.mIndices[y]);
//		}
//	}
//
//	return IndexList;
//}
//
//TextureMaps ModelLoader::LoadTextures(Renderer& renderer, const std::string& FilePath, aiMesh* mesh, const aiScene* scene)
//{
//	TextureMaps TextureList;
//
//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//	auto directory = FilePath.substr(0, FilePath.find_last_of('/')) + '/';
//
//	for (int x = 0; x <= aiTextureType_UNKNOWN; x++)
//	{
//		for (int y = 0; y <= material->GetTextureCount(static_cast<aiTextureType>(x)); y++)
//		{
//			aiString TextureLocation;
//			auto getTextureStatus = material->GetTexture(static_cast<aiTextureType>(x), y, &TextureLocation);
//			if(getTextureStatus == aiReturn_SUCCESS)
//			{
//				switch (x)
//				{
//					case aiTextureType_DIFFUSE: TextureList.DiffuseMap = Texture2D(renderer, directory + TextureLocation.C_Str()); break;
//					case aiTextureType_SPECULAR: TextureList.SpecularMap = Texture2D(renderer, directory + TextureLocation.C_Str()); break;
//					case aiTextureType_HEIGHT: TextureList.NormalMap = Texture2D(renderer, directory + TextureLocation.C_Str()); break;
//					case aiTextureType_OPACITY: TextureList.AlphaMap = Texture2D(renderer, directory + TextureLocation.C_Str()); break;
//				}
//			}
//			else
//			{
//				std::cout << "Unable to load: " << TextureLocation.C_Str() << std::endl;
//			}
//		}
//	}
//
//	if (TextureList.DiffuseMap.textureImage == VK_NULL_HANDLE)
//	{
//		TextureList.DiffuseMap = Texture2D(renderer, "texture/Temp.bmp");
//	}
//	if (TextureList.SpecularMap.textureImage == VK_NULL_HANDLE)
//	{
//		TextureList.SpecularMap = Texture2D(renderer, "texture/Temp.bmp");
//	}
//	if (TextureList.NormalMap.textureImage == VK_NULL_HANDLE)
//	{
//		TextureList.NormalMap = Texture2D(renderer, "texture/Temp.bmp");
//	}
//	if (TextureList.DisplacementMap.textureImage == VK_NULL_HANDLE)
//	{
//		TextureList.DisplacementMap = Texture2D(renderer, "texture/Temp.bmp");
//	}
//	if (TextureList.AlphaMap.textureImage == VK_NULL_HANDLE)
//	{
//		TextureList.AlphaMap = Texture2D(renderer, "texture/Temp.bmp");
//	}
//	//if (TextureList.CubeMap.textureImage == VK_NULL_HANDLE)
//	//{
//	//	CubeMapLayout layout;
//	//	layout.Left = "texture/skybox/left.jpg";
//	//	layout.Right = "texture/skybox/right.jpg";
//	//	layout.Top = "texture/skybox/top.jpg";
//	//	layout.Bottom = "texture/skybox/bottom.jpg";
//	//	layout.Back = "texture/skybox/back.jpg";
//	//	layout.Front = "texture/skybox/front.jpg";
//	//	TextureList.CubeMap = CubeMapTexture(renderer, layout);
//	//}
//	return TextureList;
//}
//
//void ModelLoader::CleanTextureMemory(Renderer& renderer)
//{
//	//for (auto subMesh : ModelMeshList)
//	//{
//	//	for (auto texture : subMesh.TextureList)
//	//	{
//	//		texture.Destroy(renderer);
//	//	}
//	//}
//}