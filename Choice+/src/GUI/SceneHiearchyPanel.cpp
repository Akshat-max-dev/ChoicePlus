#include"SceneHiearchyPanel.h"

#include <ImGuiFileDialog.h>

#include"src/Input.h"
#include"FontAwesome.h"

namespace ChoicePlus
{
	SceneHiearchyPanel::SceneHiearchyPanel()
	{
		mSelectedObject = {};
	}

	void SceneHiearchyPanel::Draw(Scene* scene)
	{
		ImGui::Begin(ICON_FK_LIST " Scene Hiearchy");
		if (scene)
		{
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
								mSelectedObject = { scene->mSceneObjects[i] };
							}
							ImGui::TreePop();
						}
						if (mSelected == i)
						{
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
									ImGui::CloseCurrentPopup();
								}
								if (ImGui::MenuItem("Rename"))mRenameSceneObjectModal = true;
								ImGui::EndPopup();
							}
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
						ImGuiFileDialog::Instance()->SetExtentionInfos(".obj", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
						ImGuiFileDialog::Instance()->OpenModal("ImportModel", "Import Model", ".obj,.fbx", ".");

					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Rename"))
				{
					mRenameSceneModal = true;
				}
				ImGui::EndPopup();
			}
		}

		if (mRenameSceneModal || mRenameSceneObjectModal)
		{
			ImGui::OpenPopup("Rename");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowSize({ 333.0f, 97.0f }, ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, { 0.5f, 0.5f });
			if (ImGui::BeginPopupModal("Rename", NULL, ImGuiWindowFlags_NoResize))
			{
				ImGui::Text("Enter Name:");
				ImGui::SameLine();
				static char buf[32] = "";
				ImGui::InputText("##InputText", buf, 32);
				ImGui::Separator();
				if (ImGui::Button("Rename") || Input::IsKeyPressed(GLFW_KEY_ENTER))
				{
					if (mRenameSceneModal)
					{
						if (strlen(buf) == 0) { CONSOLE("Scene Name Can't Be Empty{e}"); }
						else { scene->mName = buf; }
						mRenameSceneModal = false;
					}
					else if (mRenameSceneObjectModal)
					{
						if (strlen(buf) == 0) { CONSOLE("Scene Object Name Can't Be Empty{e}"); }
						else 
						{ 
							mSelectedObject.value()->Name(buf); 
							auto model = mSelectedObject.value()->GetProperty<Model>();
							if (model)
							{
								//Temp code
								std::string renameSrcFile = model->mSrcFile.substr(0, model->mSrcFile.find_last_of('/') + 1) + buf + ".cpmodel";
								rename(model->mSrcFile.c_str(), renameSrcFile.c_str());
								model->mSrcFile = renameSrcFile;
							}
						}
						mRenameSceneObjectModal = false;
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel") || Input::IsKeyPressed(GLFW_KEY_ESCAPE))
				{
					mRenameSceneModal = false;
					mRenameSceneObjectModal = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
}