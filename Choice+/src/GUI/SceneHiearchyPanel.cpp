#include"SceneHiearchyPanel.h"

#include <ImGuiFileDialog.h>

namespace ChoicePlus
{
	SceneHiearchyPanel::SceneHiearchyPanel()
	{
		mSelectedObject = {};
	}

	void SceneHiearchyPanel::Draw(Scene* scene)
	{
		ImGui::Begin("Scene Hiearchy");
		if (ImGui::CollapsingHeader(scene->mName.c_str(), mBaseFlags))
		{
			for (uint32_t i = 0; i < scene->mSceneObjects.size(); i++)
			{
				if (scene->mSceneObjects[i])
				{
					ImGuiTreeNodeFlags ParentFlags = (i == mSelected) ? ImGuiTreeNodeFlags_Selected : 0;
					ParentFlags |= ImGuiTreeNodeFlags_Leaf;
					if (ImGui::TreeNodeEx(scene->mSceneObjects[i]->Name().c_str(), ParentFlags))
					{
						if (ImGui::IsItemClicked())
						{
							mSelected = i;
							mSelectedObject.emplace(scene->mSceneObjects[i]);
						}
						ImGui::TreePop();
					}
					if (ImGui::BeginPopupContextItem())
					{
						if (ImGui::MenuItem("Delete"))
						{
							scene->DeleteObject(i);
							mSelectedObject.reset();
							ImGui::CloseCurrentPopup();
						}
						if (ImGui::MenuItem("Deselect"))
						{
							mSelected = -1;
							mSelectedObject.reset();
						}
						ImGui::EndPopup();
					}
				}
			}
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Add Model"))
			{
				if (ImGui::MenuItem("Import"))
				{
					ImGuiFileDialog::Instance()->OpenDialog("ImportModel", "Import Model", ".obj,.fbx", ".");					
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		ImGui::End();
	}
}