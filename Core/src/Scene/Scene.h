#pragma once
#include"c+pch.h"

#include"SceneObject.h"

namespace ChoicePlus
{
	class Scene
	{
	public:
		Scene(const std::string& name);
		void AddObject(const SceneObject sceneobject);
		void DeleteObject(const uint32_t index);

		std::string& Name() { return mName; }
		std::vector<SceneObject>& GetSceneObjects() { return mSceneObjects; }
	private:
		std::string mName;
		std::vector<SceneObject> mSceneObjects;
	};
}