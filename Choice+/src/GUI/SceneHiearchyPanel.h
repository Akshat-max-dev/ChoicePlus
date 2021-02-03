#pragma once
#include"c+pch.h"

#include<imgui.h>

#include"src/Scene/Scene.h"
#include"src/Scene/SceneObject.h"

namespace ChoicePlus
{
	class Scene;
}

namespace ChoicePlus
{
	class SceneHiearchyPanel
	{
	public:
		SceneHiearchyPanel();
		void Draw(Scene* scene);
		
		SceneObject* SelectedObject() { return mSelectedObject.has_value() ? mSelectedObject.value() : nullptr; }
	private:
		ImGuiTreeNodeFlags mBaseFlags = ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
		int mSelected = -1;
		std::optional<SceneObject*> mSelectedObject;
	};
}