#include"Scene.h"

namespace ChoicePlus
{
	Scene::Scene(const std::string& name) :mName(name)
	{
	}

	Scene::~Scene()
	{
		for (auto& object : mSceneObjects)
		{
			if (object)delete object;
		}
	}

	void Scene::AddObject(SceneObject* object)
	{
		mSceneObjects.emplace_back(object);
	}

	void Scene::DeleteObject(const uint32_t index)
	{
		if (mSceneObjects[index])delete mSceneObjects[index]; mSceneObjects[index] = nullptr;
	}
}