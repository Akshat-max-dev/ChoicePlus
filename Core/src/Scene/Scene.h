#pragma once
#include"c+pch.h"

#include"SceneObject.h"

namespace ChoicePlus
{
	enum class SceneObjectTypes
	{
		MODEL = 0, LIGHT = 1
	};

	class Scene
	{
	public:
		Scene(const std::string& name);
		void AddObject(const SceneObject& object);
		void DeleteObject(const uint32_t index);

		std::string& Name() { return mName; }
		std::vector<SceneObject>& GetSceneObjects() { return mSceneObjects; }
	private:
		std::string mName;
		std::vector<SceneObject> mSceneObjects;
	};
}