#pragma once
#include"c+pch.h"

#include"src/Scene/Scene.h"

namespace ChoicePlus
{
	class Project
	{
	public:
		Project(const std::string& name, const std::string& directory);
		void AddScene(Scene* scene);
		const std::string& Name()const { return mProjectName; }
		const std::string& Directory()const { return mProjectFolder; }
		const std::vector<std::string>& SceneNames()const { return mSceneNames; }
	private:
		std::vector<std::string> mSceneNames;
		std::string mProjectFolder;
		std::string mProjectName;
	};
}