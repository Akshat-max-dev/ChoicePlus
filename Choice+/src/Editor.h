#pragma once
#include"c+pch.h"
#include"Includes.h"

#include"GUI/Console.h"
#include"GUI/SceneHiearchyPanel.h"
#include"GUI/SceneInspector.h"
#include"GUI/ProjectExplorer.h"

#include"OpenGL/Pipeline.h"
#include"OpenGL/DeferredShading/DeferredPipeline.h"

namespace ChoicePlus
{
	class Console;
	class SceneHiearchyPanel;
	class SceneInspector;
	class Camera;
	class ProjectExplorer;
}

namespace ChoicePlus
{
	class Editor
	{
	public:
		Editor();
		~Editor();
		void Draw();
		void Update();

		std::unique_ptr<Camera>& CurrentCamera() { return mCamera; }
		Scene* ActiveScene() { return mProjectExplorer->ActiveScene(); }
	private:
		void SetEditorLayout();
		void NewProject();
		void SaveProject();
		void OpenProject();
	private:
		std::unique_ptr<Camera> mCamera;
		std::unique_ptr<Console> mConsole = std::make_unique<Console>();
		std::unique_ptr<SceneHiearchyPanel> mSceneHiearchyPanel = std::make_unique<SceneHiearchyPanel>();
		std::unique_ptr<SceneInspector> mSceneInspector = std::make_unique<SceneInspector>();
		std::unique_ptr<Pipeline> mPipeline = std::make_unique<DeferredPipeline>();
		std::unique_ptr<ProjectExplorer> mProjectExplorer = std::make_unique<ProjectExplorer>();
		Project* mActiveProject;
	private:
		ImGuiWindowFlags mFullscreenWindowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus | 
												  ImGuiWindowFlags_NoCollapse | 
												  ImGuiWindowFlags_NoDocking | 
												  ImGuiWindowFlags_NoFocusOnAppearing |
												  ImGuiWindowFlags_NoMove | 
												  ImGuiWindowFlags_NoNavFocus| 
												  ImGuiWindowFlags_NoResize | 
												  ImGuiWindowFlags_NoScrollbar | 
												  ImGuiWindowFlags_NoTitleBar;
		bool mViewportFullscreen = false;
		glm::vec2 mViewportSize;
	private:
		struct DockIds
		{
			uint32_t root = 0;
			uint32_t bottom = 0;
			uint32_t left = 0;
			uint32_t right = 0;
			uint32_t center = 0;
		};
		DockIds mDockIds;
		bool mNewProjectModal = false;
	};
}