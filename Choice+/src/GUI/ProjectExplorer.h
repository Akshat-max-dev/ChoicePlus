#pragma once
#include"c+pch.h"

#include"src/Project/Project.h"
#include"src/Scene/Scene.h"

namespace ChoicePlus
{
	class ProjectExplorer
	{
	public:
		ProjectExplorer();
		~ProjectExplorer();
		void Draw(Project* project);
		Scene* ActiveScene() { return mActiveScene; }
		void ActiveScene(Scene* scene) { delete mActiveScene; mActiveScene = scene; }
	private:
		void IterateDirectory(const std::string& directory);
	private:
		bool mNewSceneModal = false;
		Scene* mActiveScene;
	};
}