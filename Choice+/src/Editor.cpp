#include"Editor.h"

#include<imgui.h>
#include"src/Input.h"
#include<imgui_internal.h>

#include"src/Application.h"
#include"src/Scene/SceneContainer.h"

#include<ImGuiFileDialog.h>

#include"GUI/FontAwesome.h"

namespace ChoicePlus
{
	Editor::Editor()
	{
		mPipeline->Init();
		mActiveProject = {};
		//Temp
		mCamera = std::make_unique<EditorCamera>();
	}

	Editor::~Editor()
	{
		delete mActiveProject;
	}

	void Editor::Draw()
	{
		if (!mViewportFullscreen && Input::IsKeyPressed(GLFW_KEY_F11))
		{
			mViewportFullscreen = true;
		}
		else if (mViewportFullscreen && Input::IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			mViewportFullscreen = false;
		}

		SetEditorLayout();

		auto id = ImGui::GetID("Root_Dockspace");

		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			mFullscreenWindowFlags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Choice+ DockSpace", (bool*)0, mFullscreenWindowFlags | ImGuiWindowFlags_MenuBar);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::DockSpace(id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Project", "Ctrl + N")) { mNewProjectModal = true; }
				if (ImGui::MenuItem("Save", "Ctrl + S")) { SaveProject(); }
				if (ImGui::MenuItem("Open Project", "Ctrl + O")) { OpenProject(); }
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_N)) { mNewProjectModal = true; }
		else if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_S)) { SaveProject(); }
		else if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_O)) { OpenProject(); }

		if (!mViewportFullscreen)
		{
			if (mNewProjectModal) { NewProject(); }

			ImGui::SetNextWindowDockID(mDockIds.root, ImGuiCond_Appearing);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");

			mDockIds.root = ImGui::GetWindowDockID();

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (mViewportSize != *(glm::vec2*)&viewportSize)
			{
				mPipeline->Visible((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				mCamera->Visible((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				mViewportSize.x = viewportSize.x;
				mViewportSize.y = viewportSize.y;
			}
			void* Id = (void*)(uintptr_t)mPipeline->FinalResult();
			ImGui::Image(Id, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::SetNextWindowDockID(mDockIds.left, ImGuiCond_Appearing);
			mProjectExplorer->Draw(mActiveProject);
			ImGui::SetNextWindowDockID(mDockIds.left, ImGuiCond_Appearing);
			mSceneHiearchyPanel->Draw(mProjectExplorer->ActiveScene());
			ImGui::SetNextWindowDockID(mDockIds.right, ImGuiCond_Appearing);
			mSceneInspector->Draw(mSceneHiearchyPanel->SelectedObject());
		}
		else
		{
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			ImGui::SetNextWindowDockID(mDockIds.root, ImGuiCond_Appearing);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport", (bool*)0, mFullscreenWindowFlags);

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (mViewportSize != *(glm::vec2*)&viewportSize)
			{
				mPipeline->Visible((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				mCamera->Visible((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				mViewportSize.x = viewportSize.x;
				mViewportSize.y = viewportSize.y;
			}
			void* Id = (void*)(uintptr_t)mPipeline->FinalResult();
			ImGui::Image(Id, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar(2);
		}

		ImGui::SetNextWindowDockID(mDockIds.bottom, ImGuiCond_Appearing);
		mConsole->Draw();

		ImGui::End();
	}

	void Editor::Update()
	{
		mCamera->Update();
		if (mProjectExplorer->ActiveScene()) { mPipeline->Update(mProjectExplorer->ActiveScene(), mCamera->CameraData()); }
	}

	void Editor::SetEditorLayout()
	{
		if (ImGui::DockBuilderGetNode(mDockIds.root) == NULL) {
			mDockIds.root = ImGui::GetID("Root_Dockspace");

			ImGui::DockBuilderRemoveNode(mDockIds.root);  // Clear out existing layout
			ImGui::DockBuilderAddNode(mDockIds.root,
				ImGuiDockNodeFlags_DockSpace);  // Add empty node
			ImGui::DockBuilderSetNodeSize(mDockIds.root, ImVec2((float)Application::Get()->GetWindow()->GetWidth(),
				(float)Application::Get()->GetWindow()->GetHeight()));

			mDockIds.right = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Right,
				0.2f, NULL, &mDockIds.root);
			mDockIds.left = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Left,
				0.2f, NULL, &mDockIds.root);
			mDockIds.bottom = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Down,
				0.3f, NULL, &mDockIds.root);

			ImGui::DockBuilderDockWindow("Viewport", mDockIds.root);
			ImGui::DockBuilderDockWindow(ICON_FK_FOLDER_OPEN_O, mDockIds.left);
			ImGui::DockBuilderDockWindow(ICON_FK_TERMINAL, mDockIds.bottom);
			ImGui::DockBuilderDockWindow(ICON_FK_LIST, mDockIds.left);
			ImGui::DockBuilderDockWindow(ICON_FK_INFO_CIRCLE, mDockIds.right);
			ImGui::DockBuilderFinish(mDockIds.root);
		}
	}

	void Editor::NewProject()
	{
		ImGui::OpenPopup("Create New Project");
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowSize({ 333.0f, 130.0f }, ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, { 0.5f, 0.5f });
		if (ImGui::BeginPopupModal("Create New Project", NULL, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text("Name :");
			ImGui::SameLine();
			static char buf[32] = "";
			static std::string dirbuf;
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			ImGui::InputText("##Name", buf, 32);
			if (ImGui::BeginTable("##Directory", 3, ImGuiTableFlags_SizingFixedFit))
			{
				for (int row = 0; row < 1; row++)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Folder :");
					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(225.0f);
					ImGui::InputText("##Directory", dirbuf.data(), 256);
					ImGui::TableSetColumnIndex(2);
					if (ImGui::Button("..."))
					{
						ImGuiFileDialog::Instance()->OpenModal("ChooseDirectory", "Choose Directory", nullptr, "");
					}
				}
				ImGui::EndTable();
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseDirectory", ImGuiWindowFlags_NoCollapse, { 800, 600 }))
			{
				if (ImGuiFileDialog::Instance()->IsOk())
				{
					dirbuf = ImGuiFileDialog::Instance()->GetCurrentPath();
				}
				ImGuiFileDialog::Instance()->Close();
			}
			ImGui::Separator();
			if(ImGui::Button("Create") || Input::IsKeyPressed(GLFW_KEY_ENTER))
			{
				if (strlen(buf) == 0) { CONSOLE("Project Name Can't Be Empty{e}"); }
				else if (dirbuf.size() == 0) { CONSOLE("No Directory Selected{e}"); }
				else { delete mActiveProject; mActiveProject = new Project(buf, dirbuf); }
				mNewProjectModal = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel") || Input::IsKeyPressed(GLFW_KEY_ESCAPE))
			{
				mNewProjectModal = false;
			}
			ImGui::EndPopup();
		}
	}

	void Editor::SaveProject()
	{
		std::ofstream projfile(mActiveProject->Directory() + mActiveProject->Name() + ".cpproj", std::ios::out | std::ios::binary);
		cpassert(!projfile.is_open() && projfile.bad());

		for (auto& scenename : mActiveProject->SceneNames())
		{
			uint32_t scenenamesize = static_cast<uint32_t>(scenename.size());
			projfile.write((char*)&scenenamesize, sizeof(scenenamesize));
			projfile.write((char*)scenename.data(), scenenamesize);
		}

		projfile.close();
	}

	void Editor::OpenProject()
	{
	}

}