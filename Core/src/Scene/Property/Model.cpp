#include"Model.h"

namespace ChoicePlus
{
	Model::~Model()
	{
		mMeshes.clear();
	}

	void Model::Load(const std::string& fromFile, const std::string& toFile)
	{
		mFomFile = fromFile; mToFile = toFile;
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fromFile, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
		cpassert(scene);
		cpassert(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
		cpassert(scene->mRootNode);

		ProcessNode(scene->mRootNode, scene);

		importer.FreeScene();
	}

	void Model::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			ProcessMesh(scene->mMeshes[i]);
		}
	}

	void Model::ProcessMesh(const aiMesh* mesh)
	{
		std::vector<float> vertices;
		std::vector<uint32_t> indices;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(mesh->mVertices[i].x);
			vertices.push_back(mesh->mVertices[i].y);
			vertices.push_back(mesh->mVertices[i].z);

			if (mesh->mNormals)
			{
				vertices.push_back(mesh->mNormals[i].x);
				vertices.push_back(mesh->mNormals[i].y);
				vertices.push_back(mesh->mNormals[i].z);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				vertices.push_back(mesh->mTextureCoords[0][i].x);
				vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}

			if (mesh->mTangents)
			{
				vertices.push_back(mesh->mTangents[i].x);
				vertices.push_back(mesh->mTangents[i].y);
				vertices.push_back(mesh->mTangents[i].z);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}

			if (mesh->mBitangents)
			{
				vertices.push_back(mesh->mBitangents[i].x);
				vertices.push_back(mesh->mBitangents[i].y);
				vertices.push_back(mesh->mBitangents[i].z);
			}
			else
			{
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		uint32_t materialIndex = mesh->mMaterialIndex;

		std::shared_ptr<VertexArray> vertexarray = std::make_shared<VertexArray>();

		vertexarray->Bind();

		std::shared_ptr<Buffer<BufferType::VERTEX>> VertexBuffer(new Buffer<BufferType::VERTEX>(vertices.data(), (uint32_t)vertices.size() * sizeof(float)));
		vertexarray->Setup(VertexBuffer, "{3} {3} {2} {3} {3}");

		std::shared_ptr<Buffer<BufferType::INDEX>> IndexBuffer(new Buffer<BufferType::INDEX>(indices.data(), (uint32_t)indices.size()));
		vertexarray->SetIndexBuffer({ IndexBuffer });

		mMeshes.push_back({ vertexarray, materialIndex });
	}
}