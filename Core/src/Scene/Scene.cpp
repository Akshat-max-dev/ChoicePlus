#include"Scene.h"

namespace ChoicePlus
{
	Scene::Scene(const std::string& name) :mName(name)
	{
	}

	void Scene::AddObject(const SceneObject sceneobject)
	{
		mSceneObjects.push_back(sceneobject);
	}

	void Scene::DeleteObject(const uint32_t index)
	{
		mSceneObjects.erase(mSceneObjects.begin(), mSceneObjects.begin() + index);
	}
}