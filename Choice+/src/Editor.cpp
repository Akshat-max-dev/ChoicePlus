#include"Editor.h"

#include<imgui.h>

namespace ChoicePlus
{
	Editor::Editor()
	{
		mPipeline->Init();
		mActiveScene = new Scene("Choice+");

		//Temp
		mCamera = std::make_unique<EditorCamera>();

		Model model;
		model.Load("G:/Crytek Sponza/Sponza/sponza.obj", "");
		SceneObject sceneobject;
		sceneobject.AddProperty<Model>(model);
		mActiveScene->AddObject(sceneobject);
	}

	Editor::~Editor()
	{
		delete mActiveScene;
	}

	void Editor::Draw()
	{
		static bool p_open = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Choice+ DockSpace", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("Choice DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		mSceneHiearchyPanel->Draw();
		mSceneInspector->Draw();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		if (mViewportSize != *(glm::vec2*)&viewportSize)
		{
			mViewportSize.x = viewportSize.x;
			mViewportSize.y = viewportSize.y;
		}

		uint32_t Id = mPipeline->FinalResult();
		ImGui::Image((void*)(Id), ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		ImGui::End();
		ImGui::PopStyleVar();

		mConsole->Draw();

		ImGui::End();
	}

	void Editor::Update()
	{
		mCamera->Update();
		mPipeline->Update(mActiveScene, mCamera->CameraData());
	}
}