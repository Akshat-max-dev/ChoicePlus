#include"ProjectExplorer.h"

#include<imgui.h>
#include"FontAwesome.h"

#include"src/Input.h"
#include"src/Scene/SceneContainer.h"

namespace ChoicePlus
{
	ProjectExplorer::ProjectExplorer()
	{
		mActiveScene = {};
	}

	ProjectExplorer::~ProjectExplorer()
	{
		delete mActiveScene;
	}

	void ProjectExplorer::Draw(Project* project)
	{
		ImGui::Begin(ICON_FK_FOLDER_OPEN_O);
		if (project)
		{
			if (ImGui::TreeNodeEx(project->Name().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
			{
				IterateDirectory(project->Directory());
				ImGui::TreePop();
			}

			if (ImGui::IsWindowFocused() && (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_N)))
			{
				mNewSceneModal = true;
			}

			if (ImGui::BeginPopupContextWindow(0, 1, false))
			{
				if (ImGui::MenuItem("New Scene")) { mNewSceneModal = true; }
				ImGui::EndPopup();
			}
		}

		if (mNewSceneModal)
		{
			ImGui::OpenPopup("New Scene");
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowSize({ 333.0f, 97.0f }, ImGuiCond_Appearing);
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, { 0.5f, 0.5f });
			if (ImGui::BeginPopupModal("New Scene", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
			{
				ImGui::Text("Name :"); ImGui::SameLine();
				static char buf[32] = "";
				ImGui::InputText("##Name", buf, 32);
				ImGui::Separator();
				if (ImGui::Button("Create") || Input::IsKeyPressed(GLFW_KEY_ENTER))
				{
					if (strlen(buf) == 0) { CONSOLE("Scene Name Can't Be Empty{e}"); }
					else
					{
						Scene* scene = new Scene(buf);
						project->AddScene(scene);
						scene->Directory(project->Directory() + buf + "\\");
						delete scene;

						if (!mActiveScene)
						{
							std::unique_ptr<SceneContainer> scenecontainer = std::make_unique<SceneContainer>();
							std::string loadscene = project->Directory() + project->SceneNames()[0] + "\\" + project->SceneNames()[0] + ".cpscene";
							mActiveScene = scenecontainer->LoadContainedScene(loadscene);
							project->ActiveScene(mActiveScene->Name());
						}
					}
					mNewSceneModal = false;
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel") || Input::IsKeyPressed(GLFW_KEY_ESCAPE))
				{
					mNewSceneModal = false;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();

	}

	void ProjectExplorer::IterateDirectory(const std::string& directory)
	{
		for (auto& it : std::filesystem::directory_iterator(directory))
		{
			auto& f = it.path();
			if (std::filesystem::is_directory(f))
			{
				std::string folder = ICON_FK_FOLDER_O;
				if (ImGui::TreeNodeEx((folder + " " + f.filename().string()).c_str(), ImGuiTreeNodeFlags_OpenOnArrow))
				{
					IterateDirectory(f.string());
					ImGui::TreePop();
				}
			}
			else
			{
				std::string ext = f.filename().extension().string();
				if (!(ext == ".cpproj" || ext == ".cpscene"))
				{
					std::string icon = ICON_FK_FILE_O;
					if (ext == ".dds")icon = ICON_FK_PICTURE_O;
					if (ImGui::TreeNodeEx((icon + " " + f.filename().string()).c_str(), ImGuiTreeNodeFlags_Leaf))
					{
						ImGui::TreePop();
					}
				}
			}
		}
	}
}