#pragma once
#include"c+pch.h"

#include"src/Scene/SceneObject.h"

#include<glm/glm.hpp>

namespace ChoicePlus
{
	class SceneInspector
	{
	public:
		void Draw(SceneObject* sceneobject);
	};
}