
#pragma once
#include"c+pch.h"

#include"SceneObject.h"
#include"GUI/SceneHiearchyPanel.h"

namespace ChoicePlus
{
	class Scene
	{
	public:
		Scene(const std::string& name);
		~Scene();
		void AddObject(SceneObject* object);
		void DeleteObject(const uint32_t index);

		//std::string& Name() { return mName; }
		std::vector<SceneObject*>& GetSceneObjects() { return mSceneObjects; }
	private:
		std::string mName;
		std::vector<SceneObject*> mSceneObjects;
	private:
		friend class SceneHiearchyPanel;
	};
}