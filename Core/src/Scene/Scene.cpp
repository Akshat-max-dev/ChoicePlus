#include"Scene.h"

namespace ChoicePlus
{
	Scene::Scene(const std::string& name) :mName(name)
	{
	}

	void Scene::AddObject(const SceneObject& object)
	{
		mSceneObjects.push_back(object);
	}

	void Scene::DeleteObject(const uint32_t index)
	{
		mSceneObjects.erase(mSceneObjects.begin(), mSceneObjects.begin() + index);
	}
}