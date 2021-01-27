#pragma once
#include"c+pch.h"

#include"src/Log.h"

#include"OpenGL/Buffer.h"
#include"OpenGL/VertexArray.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"OpenGL/Texture.h"

namespace ChoicePlus
{
	struct Material
	{
		~Material()
		{
			delete mDiffuseMap;
			delete mNormalMap;
		}
		
		Texture<TextureTypes::TWO_D>* mDiffuseMap;
		Texture<TextureTypes::TWO_D>* mNormalMap;
	};

	struct DumpableMeshData
	{
		std::vector<float> Vertices;
		std::vector<uint32_t> Indices;
		uint32_t MaterialIndex;
	};

	struct DumpableMaterialData
	{
		std::string DiffuseMap;
		std::string NormalMap;
	};

	const std::string DumpModel(const std::string& srcFile, const std::string& dstDirectory);

	class Model
	{
	public:
		~Model();
		void Load(const std::string& srcFile);

		const std::string& Name()const { return mName; }
		const std::string& GetPath()const { return mSrcFile; }

		const std::vector<std::shared_ptr<Material>>& GetMaterials()const { return mMaterials; }
		const std::vector<std::pair<std::shared_ptr<VertexArray>, uint32_t>>& GetMeshes()const { return mMeshes; }
	private:
		std::string mName;
		std::vector<std::shared_ptr<Material>> mMaterials;
		std::vector<std::pair<std::shared_ptr<VertexArray>, uint32_t>> mMeshes;
		std::string mSrcFile;
	};
}