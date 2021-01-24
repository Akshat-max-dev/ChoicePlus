#pragma once
#include"c+pch.h"

#include"src/Log.h"

#include"OpenGL/Buffer.h"
#include"OpenGL/VertexArray.h"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

namespace ChoicePlus
{
	class Model
	{
	public:
		~Model();
		void Load(const std::string& fromFile, const std::string& toFile);

		const std::vector<std::pair<std::shared_ptr<VertexArray>, uint32_t>>& GetMeshes()const { return mMeshes; }
	private:
		void ProcessNode(const aiNode* node, const aiScene* scene);
		void ProcessMesh(const aiMesh* mesh);
	private:
		std::vector<std::pair<std::shared_ptr<VertexArray>, uint32_t>> mMeshes;
		std::string mFomFile, mToFile;
	};
}