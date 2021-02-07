#include"Editor.h"

#include<imgui.h>
#include"src/Input.h"
#include<imgui_internal.h>

#include"src/Application.h"
#include"src/Scene/SceneContainer.h"

#include<ImGuiFileDialog.h>

#pragma warning(push)
#pragma warning(disable : 4312)
#pragma warning(disable : 4244)

namespace ChoicePlus
{
	Editor::Editor()
	{
		mPipeline->Init();
		mActiveScene = {};
		//Temp
		mCamera = std::make_unique<EditorCamera>();
	}

	Editor::~Editor()
	{
		delete mActiveScene;
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
				if (ImGui::MenuItem("New Scene"))
				{
					mNewSceneModal = true;
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					std::unique_ptr<SceneContainer> scenecontainer = std::make_unique<SceneContainer>();
					std::string scenepath = "E:/Choice+/Choice+/assets/" + mActiveScene->Name() + ".cpscene";
					scenecontainer->ContainScene(mActiveScene, scenepath);
				}
				if (ImGui::MenuItem("Load Scene"))
				{
					ImGuiFileDialog::Instance()->SetExtentionInfos(".cpscene", ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGuiFileDialog::Instance()->OpenModal("LoadScene", "Load Scene", ".cpscene", ".");
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (!mViewportFullscreen)
		{
			if (mNewSceneModal)
			{
				ImGui::OpenPopup("Create New Scene");
				ImVec2 center = viewport->GetCenter();
				ImGui::SetNextWindowSize({ 333.0f, 97.0f }, ImGuiCond_Appearing);
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, { 0.5f, 0.5f });
				if (ImGui::BeginPopupModal("Create New Scene", NULL, ImGuiWindowFlags_NoResize))
				{
					ImGui::Text("Scene Name:");
					ImGui::SameLine();
					static char buf[32] = "";
					ImGui::InputText("##SceneName", buf, 32);
					ImGui::Separator();
					if (ImGui::Button("Create") || Input::IsKeyPressed(GLFW_KEY_ENTER))
					{
						if (strlen(buf) == 0) { CONSOLE("Scene Name Can't Be Empty{e}"); }
						else { delete mActiveScene; mActiveScene = new Scene(buf); }
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
			void* Id = (void*)mPipeline->FinalResult();
			ImGui::Image(Id, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::SetNextWindowDockID(mDockIds.left, ImGuiCond_Appearing);
			mSceneHiearchyPanel->Draw(mActiveScene);
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
			void* Id = (void*)mPipeline->FinalResult();
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
		if (mActiveScene)
		{
			mPipeline->Update(mActiveScene, mCamera->CameraData());
		}
	}

	void Editor::SetEditorLayout()
	{
		if (ImGui::DockBuilderGetNode(mDockIds.root) == NULL) {
			mDockIds.root = ImGui::GetID("Root_Dockspace");

			ImGui::DockBuilderRemoveNode(mDockIds.root);  // Clear out existing layout
			ImGui::DockBuilderAddNode(mDockIds.root,
				ImGuiDockNodeFlags_DockSpace);  // Add empty node
			ImGui::DockBuilderSetNodeSize(mDockIds.root, ImVec2(Application::Get()->GetWindow()->GetWidth(),
				Application::Get()->GetWindow()->GetHeight()));

			mDockIds.right = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Right,
				0.2f, NULL, &mDockIds.root);
			mDockIds.left = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Left,
				0.2f, NULL, &mDockIds.root);
			mDockIds.bottom = ImGui::DockBuilderSplitNode(mDockIds.root, ImGuiDir_Down,
				0.3f, NULL, &mDockIds.root);

			ImGui::DockBuilderDockWindow("Viewport", mDockIds.root);
			ImGui::DockBuilderDockWindow("Console", mDockIds.bottom);
			ImGui::DockBuilderDockWindow("Scene Hierarchy", mDockIds.left);
			ImGui::DockBuilderDockWindow("Scene Inspector", mDockIds.right);
			ImGui::DockBuilderFinish(mDockIds.root);
		}
	}

}
#pragma warning(pop)