#include"Editor.h"

#include<imgui.h>
#include"src/Input.h"
#include<imgui_internal.h>

#include"src/Application.h"

namespace ChoicePlus
{
	Editor::Editor()
	{
		mPipeline->Init();
		mActiveScene = new Scene("Choice+");

		//Temp
		mCamera = std::make_unique<EditorCamera>();

		mSceneHiearchyPanel->ActiveScene(mActiveScene);
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
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (!mViewportFullscreen)
		{
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
			mSceneHiearchyPanel->Draw();
			ImGui::SetNextWindowDockID(mDockIds.right, ImGuiCond_Appearing);
			mSceneInspector->Draw();
			ImGui::SetNextWindowDockID(mDockIds.bottom, ImGuiCond_Appearing);
			mConsole->Draw();
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

		ImGui::End();
	}

	void Editor::Update()
	{
		mCamera->Update();
		mPipeline->Update(mActiveScene, mCamera->CameraData());
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