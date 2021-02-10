#pragma once
#include"c+pch.h"

#include"src/Log.h"

#include"OpenGL/VertexArray.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include"OpenGL/Texture.h"

#include<glm/glm.hpp>
#include<glad/glad.h>

namespace ChoicePlus
{
	struct Material
	{
		~Material()
		{
			glDeleteTextures(1, &mDiffuseMap.first);
			glDeleteTextures(1, &mNormalMap.first);
		}

		void Bind(glm::uvec2 slots)const
		{
			glActiveTexture(GL_TEXTURE0 + slots.x); glBindTexture(GL_TEXTURE_2D, mDiffuseMap.first);
			glActiveTexture(GL_TEXTURE0 + slots.y); glBindTexture(GL_TEXTURE_2D, mNormalMap.first);
		}

		std::pair<uint32_t, std::string> mDiffuseMap;
		std::pair<uint32_t, std::string> mNormalMap;
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

	struct Model
	{
		~Model();

		std::string mName;
		std::vector<Material*> mMaterials;
		std::vector<std::pair<std::shared_ptr<VertexArray>, uint32_t>> mMeshes;
		std::string mSrcFile;
	};

	const std::string DumpModel(const std::string& srcFile, const std::string& dstDirectory);
	Model* Load(const std::string& srcFile);
}