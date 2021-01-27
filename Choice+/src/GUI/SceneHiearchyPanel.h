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
		void Draw();
		void ActiveScene(Scene* scene);
	private:
		ImGuiTreeNodeFlags mBaseFlags = ImGuiTreeNodeFlags_CollapsingHeader | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
		int mSelected = -1;
		Scene* mScene;
		SceneObject mSelectedObject;
	};
}