#pragma once
#include"c+pch.h"

#include"Scene.h"

namespace ChoicePlus
{
	class Scene;
	class SceneContainer
	{
	public:
		void ContainScene(Scene* scene, const std::string& scenepath);
		Scene* LoadContainedScene(const std::string& scenepath);
	};
}