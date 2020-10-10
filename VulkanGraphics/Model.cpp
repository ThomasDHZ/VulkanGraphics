#include "Model.h"

Model::Model()
{
}

Model::Model(RendererManager& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath)
{
	LoadModel(renderer, textureManager, FilePath);

	for (auto mesh : SubMeshList)
	{
		MeshList.emplace_back(Mesh(renderer, textureManager, mesh));
	}

	SendDrawMessage(renderer);
}

Model::~Model()
{

}

void Model::LoadModel(VulkanEngine& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath)
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

void Model::ProcessNode(VulkanEngine& renderer, std::shared_ptr<TextureManager>& textureManager, const std::string& FilePath, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		MeshData NewMesh;
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		NewMesh.VertexList = LoadVertices(mesh);
		NewMesh.IndexList = LoadIndices(mesh);
		NewMesh.TextureList = LoadTextures(renderer, textureManager, FilePath, mesh, scene);
		BoneList = LoadBones(scene->mRootNode, mesh, NewMesh.VertexList);
		LoadAnimations(scene);
		SubMeshList.emplace_back(NewMesh);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(renderer, textureManager, FilePath, node->mChildren[i], scene);
	}
}

std::vector<Vertex> Model::LoadVertices(aiMesh* mesh)
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
		else
		{
			vertex.Tangant = glm::vec3{ 0.0f, 0.0f, 0.0f };
			vertex.BiTangant = glm::vec3{ 0.0f, 0.0f, 0.0f };
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.TexureCoord = glm::vec2{ mesh->mTextureCoords[0][x].x, mesh->mTextureCoords[0][x].y };
		}
		else
		{
			vertex.TexureCoord = glm::vec2{ 0.0f, 0.0f };
		}

		vertex.BoneID = glm::vec4(0.0f);
		vertex.BoneWeights = glm::vec4(0.0f);

		VertexList.emplace_back(vertex);
	}

	return VertexList;
}

std::vector<uint16_t> Model::LoadIndices(aiMesh* mesh)
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

std::vector<Bone> Model::LoadBones(const aiNode* RootNode, const aiMesh* mesh, std::vector<Vertex>& VertexList)
{
	std::vector<Bone> BoneList;
	for (int x = 0; x < mesh->mNumBones; x++)
	{
		BoneList.emplace_back(Bone(mesh->mBones[x]->mName.data, x, AssimpToGLMMatrixConverter(mesh->mBones[x]->mOffsetMatrix)));
	}

	for (Bone& bone : BoneList)
	{
		auto BoneAsNode = RootNode->FindNode(bone.GetBoneName().c_str());
		for (int x = 0; x < BoneAsNode->mNumChildren; x++)
		{
			for (Bone& bone2 : BoneList)
			{
				if (bone2.GetBoneName() == BoneAsNode->mChildren[x]->mName.C_Str())
				{
					bone.AddChildBone(bone2);
				}
			}
		}
	}

	for (int x = 0; x < mesh->mNumBones; x++)
	{
		std::vector<unsigned int> AffectedVertices;
		AffectedVertices.resize(VertexList.size(), 0);

		aiBone* bone = mesh->mBones[x];
		for (int y = 0; y < bone->mNumWeights; y++)
		{
			unsigned int vertexID = bone->mWeights[y].mVertexId;
			float weight = bone->mWeights[y].mWeight;
			AffectedVertices[vertexID]++;
			switch (AffectedVertices[vertexID])
			{
				case 1:
					VertexList[vertexID].BoneID.x = vertexID;
					VertexList[vertexID].BoneWeights.x = weight;
					break;
				case 2:
					VertexList[vertexID].BoneID.y = vertexID;
					VertexList[vertexID].BoneWeights.y = weight;
					break;
				case 3:
					VertexList[vertexID].BoneID.z = vertexID;
					VertexList[vertexID].BoneWeights.z = weight;
					break;
				case 4:
					VertexList[vertexID].BoneID.w = vertexID;
					VertexList[vertexID].BoneWeights.w = weight;
					break;
			}
		}
	}

	for (int x = 0; x < VertexList.size(); x++)
	{
		float Weight = VertexList[x].BoneWeights.x +
			VertexList[x].BoneWeights.y +
			VertexList[x].BoneWeights.z +
			VertexList[x].BoneWeights.w;
		if (Weight != 1.0f)
		{
			VertexList[x].BoneWeights = glm::vec4(
			VertexList[x].BoneWeights.x / Weight,
			VertexList[x].BoneWeights.y / Weight,
			VertexList[x].BoneWeights.z / Weight,
			VertexList[x].BoneWeights.w / Weight);
		}
	}

	return BoneList;
}

std::vector<Animation3D> Model::LoadAnimations(const aiScene* scene)
{
	for (int x = 0; x < scene->mNumAnimations; x++)
	{
		aiAnimation* assImpAnimation = scene->mAnimations[x];

		Animation3D animation = Animation3D();
		animation.TicksPerSec = assImpAnimation->mTicksPerSecond;
		animation.AnimationTime = assImpAnimation->mDuration * animation.TicksPerSec;

		for (int y = 0; y < assImpAnimation->mNumChannels; y++)
		{
			aiNodeAnim* channel = assImpAnimation->mChannels[y];
			for (int z = 0; z < channel->mNumPositionKeys; z++)
			{
				auto a = channel->mPositionKeys[z].mTime;
				auto b = channel->mPositionKeys[z].mValue;
				int c = 34;
			}
		}

		AnimationList.emplace_back(animation);
	}
	return AnimationList;
}

MeshTextures Model::LoadTextures(VulkanEngine& renderer, std::shared_ptr<TextureManager> textureManager, const std::string& FilePath, aiMesh* mesh, const aiScene* scene)
{
	MeshTextures meshTextures;

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto directory = FilePath.substr(0, FilePath.find_last_of('/')) + '/';

	meshTextures.DiffuseMap = "C:/Users/dotha/source/repos/VulkanGraphics/VulkanGraphics/Models/TestAnimModel/diffuse.png";
	meshTextures.SpecularMap = DefaultTexture;
	meshTextures.NormalMap = DefaultTexture;
	meshTextures.AlphaMap = DefaultTexture;
	meshTextures.DepthMap = DefaultTexture;
	meshTextures.ReflectionMap = DefaultTexture;
	meshTextures.CubeMap[0] = DefaultTexture;
	meshTextures.CubeMap[1] = DefaultTexture;
	meshTextures.CubeMap[2] = DefaultTexture;
	meshTextures.CubeMap[3] = DefaultTexture;
	meshTextures.CubeMap[4] = DefaultTexture;
	meshTextures.CubeMap[5] = DefaultTexture;

	//auto a = material->GetTextureCount(aiTextureType_DIFFUSE);
	//auto b = material->GetTextureCount(aiTextureType_SPECULAR);
	//auto c = material->GetTextureCount(aiTextureType_HEIGHT);
	//auto d = material->GetTextureCount(aiTextureType_AMBIENT);
	//auto e = material->GetTextureCount(aiTextureType_OPACITY);


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
	return meshTextures;
}

void Model::SendDrawMessage(RendererManager& renderer)
{
	for (auto mesh : MeshList)
	{
		mesh.CreateDrawMessage(renderer, 4, renderer.sceneRenderer.renderer3DPipeline);
	}
}

glm::mat4 Model::AssimpToGLMMatrixConverter(aiMatrix4x4 AssMatrix)
{
	glm::mat4 GLMMatrix;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			GLMMatrix[x][y] = AssMatrix[y][x];
		}
	}
	return GLMMatrix;
}

void Model::UpdateUniformBuffer(RendererManager& renderer, std::shared_ptr<Camera>& camera, LightBufferObject& light)
{
	for (auto mesh : MeshList)
	{
		mesh.Update(renderer, camera, light);
	}
}

void Model::Destroy(RendererManager& renderer)
{
	for (auto mesh : MeshList)
	{
		mesh.Destory(renderer);
	}
}
