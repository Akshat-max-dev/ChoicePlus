#include"SceneHiearchyPanel.h"

#include"src/FileDialogs.h"

namespace ChoicePlus
{
	SceneHiearchyPanel::SceneHiearchyPanel()
	{
		mSelectedObject = {};
	}

	void SceneHiearchyPanel::Draw()
	{
		ImGui::Begin("Scene Hiearchy");
		if (ImGui::CollapsingHeader(mScene->mName.c_str(), mBaseFlags))
		{
			for (uint32_t i = 0; i < mScene->mSceneObjects.size(); i++)
			{
				ImGuiTreeNodeFlags ParentFlags = (i == mSelected) ? ImGuiTreeNodeFlags_Selected : 0;
				ParentFlags |= ImGuiTreeNodeFlags_Leaf;
				if (ImGui::TreeNodeEx(mScene->mSceneObjects[i].Name().c_str(), ParentFlags))
				{
					if (ImGui::IsItemClicked())
					{
						mSelected = i;
						mSelectedObject = mScene->mSceneObjects[i];
					}
					ImGui::TreePop();
				}
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						mScene->DeleteObject(i);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Add Model"))
			{
				if (ImGui::MenuItem("Import"))
				{
					std::optional<std::string> openfile = FileDialogs::OpenFile("All Files");
					if (openfile.has_value())
					{
						std::string dumpedmodelsrc = DumpModel(openfile.value(), "E:/Choice+/Choice+/assets/models/");
						Model model;
						model.Load(dumpedmodelsrc);
						SceneObject sceneobject;
						sceneobject.AddProperty<Model>(model);
						mScene->AddObject(sceneobject);
					}					
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void SceneHiearchyPanel::ActiveScene(Scene* scene)
	{
		mScene = scene;
	}

}