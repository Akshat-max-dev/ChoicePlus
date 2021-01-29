#include"Model.h"

namespace ChoicePlus
{
	std::string GetTextureName(const aiMaterial* mat, const aiTextureType type)
	{
		if (mat->GetTextureCount(type) > 0)
		{
			aiString path;
			mat->GetTexture(type, 0, &path);
			return path.C_Str();
		}
		return {};
	}

	const DumpableMeshData ProcessMesh(const aiMesh* mesh)
	{
		DumpableMeshData data;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			data.Vertices.push_back(mesh->mVertices[i].x);
			data.Vertices.push_back(mesh->mVertices[i].y);
			data.Vertices.push_back(mesh->mVertices[i].z);

			if (mesh->mNormals)
			{
				data.Vertices.push_back(mesh->mNormals[i].x);
				data.Vertices.push_back(mesh->mNormals[i].y);
				data.Vertices.push_back(mesh->mNormals[i].z);
			}
			else
			{
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
			}

			if (mesh->mTextureCoords[0])
			{
				data.Vertices.push_back(mesh->mTextureCoords[0][i].x);
				data.Vertices.push_back(mesh->mTextureCoords[0][i].y);
			}
			else
			{
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
			}

			if (mesh->mTangents)
			{
				data.Vertices.push_back(mesh->mTangents[i].x);
				data.Vertices.push_back(mesh->mTangents[i].y);
				data.Vertices.push_back(mesh->mTangents[i].z);
			}
			else
			{
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
			}

			if (mesh->mBitangents)
			{
				data.Vertices.push_back(mesh->mBitangents[i].x);
				data.Vertices.push_back(mesh->mBitangents[i].y);
				data.Vertices.push_back(mesh->mBitangents[i].z);
			}
			else
			{
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
				data.Vertices.push_back(0.0f);
			}
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				data.Indices.push_back(face.mIndices[j]);
			}
		}

		data.MaterialIndex = mesh->mMaterialIndex;
		return data;
	}

	const std::vector<DumpableMeshData> ProcessNode(const aiNode* node, const aiScene* scene)
	{
		std::vector<DumpableMeshData> data;
		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			data.push_back(ProcessMesh(scene->mMeshes[i]));
		}
		return data;
	}

	const std::string DumpModel(const std::string& srcFile, const std::string& dstDirectory)
	{
		std::string& name = std::filesystem::path(srcFile).stem().string();
		name.append(".cpmodel");
		std::string dstFile = dstDirectory + name;

		if (std::filesystem::exists(std::filesystem::path(dstFile)))return dstFile;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(srcFile,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_RemoveRedundantMaterials);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::string msg = importer.GetErrorString();
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return {};
		}

		std::string srcDirectory = srcFile.substr(0, srcFile.find_last_of('\\') + 1);

		std::vector<DumpableMaterialData> matData(scene->mNumMaterials);

		std::string tdstDirectory = dstDirectory;

		for (uint32_t i = 0; i < scene->mNumMaterials; i++)
		{
			auto mat = scene->mMaterials[i];

			std::string diffuse_map = GetTextureName(mat, aiTextureType_DIFFUSE);
			if (!diffuse_map.empty())
			{
				std::string texdiffuse = CompressTexture(srcDirectory + diffuse_map, tdstDirectory, BlockCompressionFormat::BC1, true);
				if (!texdiffuse.empty())matData[i].DiffuseMap = texdiffuse;
			}

			tdstDirectory = dstDirectory;

			std::string normal_map = GetTextureName(mat, aiTextureType_HEIGHT);
			if (!normal_map.empty())
			{
				std::string texnormal = CompressTexture(srcDirectory + normal_map, tdstDirectory, BlockCompressionFormat::BC5, true);
				if (!texnormal.empty())matData[i].NormalMap = texnormal;
			}
		}

		std::vector<DumpableMeshData> data = ProcessNode(scene->mRootNode, scene);

		importer.FreeScene();

		std::ofstream toFile;
		toFile.open(dstFile, std::ios::out | std::ios::binary);
		if (toFile.fail() && !toFile.good())
		{
			std::string msg = "Failed to dump " + srcFile;
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return {};
		}

		uint32_t namesize = (uint32_t)name.size();
		toFile.write((char*)&namesize, sizeof(namesize));
		toFile.write((char*)name.data(), namesize);

		uint32_t matsize = (uint32_t)matData.size();
		toFile.write((char*)&matsize, sizeof(matsize));
		for (uint32_t i = 0; i < matsize; i++)
		{
			uint32_t texdiffusesize = (uint32_t)matData[i].DiffuseMap.size();
			toFile.write((char*)&texdiffusesize, sizeof(texdiffusesize));
			toFile.write((char*)matData[i].DiffuseMap.data(), texdiffusesize);

			uint32_t texnormalsize = (uint32_t)matData[i].NormalMap.size();
			toFile.write((char*)&texnormalsize, sizeof(texnormalsize));
			toFile.write((char*)matData[i].NormalMap.data(), texnormalsize);
		}

		uint32_t meshessize = (uint32_t)data.size();
		toFile.write((char*)&meshessize, sizeof(meshessize));
		for (uint32_t i = 0; i < meshessize; i++)
		{
			uint32_t verticessize = data[i].Vertices.size();
			toFile.write((char*)&verticessize, sizeof(verticessize));
			toFile.write((char*)data[i].Vertices.data(), verticessize * sizeof(float));

			uint32_t indicessize = (uint32_t)data[i].Indices.size();
			toFile.write((char*)&indicessize, sizeof(indicessize));
			toFile.write((char*)data[i].Indices.data(), indicessize * sizeof(uint32_t));

			uint32_t materialindex = data[i].MaterialIndex;
			toFile.write((char*)&materialindex, sizeof(materialindex));
		}

		toFile.close();

		return dstFile;
	}

	Model::~Model()
	{
		mMaterials.clear();
		mMeshes.clear();
	}

	void Model::Load(const std::string& srcFile)
	{
		mSrcFile = srcFile;

		std::ifstream fromFile(srcFile, std::ios::in | std::ios::binary);
		if (fromFile.fail() && !fromFile.good())
		{
			std::string msg = "Failed to load " + srcFile;
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return;
		}

		uint32_t namesize;
		fromFile.read((char*)&namesize, sizeof(namesize));
		mName.resize(namesize);
		fromFile.read((char*)mName.data(), namesize);

		mName = mName.substr(0, mName.find_last_of('.'));

		uint32_t matSize;
		fromFile.read((char*)&matSize, sizeof(matSize));
		mMaterials.resize(matSize);

		for (uint32_t i = 0; i < matSize; i++)
		{
			mMaterials[i] = std::make_shared<Material>();

			uint32_t texdiffusesize;
			fromFile.read((char*)&texdiffusesize, sizeof(texdiffusesize));
			std::string diffusemap;
			diffusemap.resize(texdiffusesize);
			fromFile.read((char*)diffusemap.data(), texdiffusesize);
			mMaterials[i]->mDiffuseMap = new Texture<TextureTypes::TWO_D>(diffusemap);

			uint32_t texnormalsize;
			fromFile.read((char*)&texnormalsize, sizeof(texnormalsize));
			std::string normalmap;
			normalmap.resize(texnormalsize);
			fromFile.read((char*)normalmap.data(), texnormalsize);
			mMaterials[i]->mNormalMap = new Texture<TextureTypes::TWO_D>(normalmap);
		}

		uint32_t meshSize;
		fromFile.read((char*)&meshSize, sizeof(meshSize));
		mMeshes.resize(meshSize);

		for (uint32_t i = 0; i < meshSize; i++)
		{
			uint32_t numvertices;
			fromFile.read((char*)&numvertices, sizeof(numvertices));
			std::vector<float> vertices(numvertices);
			fromFile.read((char*)vertices.data(), numvertices * sizeof(float));

			uint32_t numindices;
			fromFile.read((char*)&numindices, sizeof(numindices));
			std::vector<uint32_t> indices(numindices);
			fromFile.read((char*)indices.data(), numindices * sizeof(uint32_t));

			uint32_t materialindex;
			fromFile.read((char*)&materialindex, sizeof(materialindex));

			mMeshes[i].first = std::make_shared<VertexArray>();

			mMeshes[i].first->Bind();

			std::shared_ptr<Buffer<BufferType::VERTEX>> vertexbuffer(new Buffer<BufferType::VERTEX>(vertices.data(), numvertices * sizeof(float)));
			mMeshes[i].first->Setup(vertexbuffer, "{3} {3} {2} {3} {3}");

			std::shared_ptr<Buffer<BufferType::INDEX>> indexbuffer(new Buffer<BufferType::INDEX>(indices.data(), numindices));
			mMeshes[i].first->SetIndexBuffer(indexbuffer);

			mMeshes[i].second = materialindex;
		}

		fromFile.close();
	}
}