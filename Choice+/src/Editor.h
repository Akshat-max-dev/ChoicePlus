#pragma once
#include"c+pch.h"
#include"Includes.h"

#include"GUI/Console.h"
#include"GUI/SceneHiearchyPanel.h"
#include"GUI/SceneInspector.h"

#include"OpenGL/Pipeline.h"
#include"OpenGL/DeferredShading/DeferredPipeline.h"

namespace ChoicePlus
{
	class Console;
	class SceneHiearchyPanel;
	class SceneInspector;
	class Camera;
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
	private:
		std::unique_ptr<Camera> mCamera;
		std::unique_ptr<Console> mConsole = std::make_unique<Console>();
		std::unique_ptr<SceneHiearchyPanel> mSceneHiearchyPanel = std::make_unique<SceneHiearchyPanel>();
		std::unique_ptr<SceneInspector> mSceneInspector = std::make_unique<SceneInspector>();
		std::unique_ptr<Pipeline> mPipeline = std::make_unique<DeferredPipeline>();
		Scene* mActiveScene;
	private:
		glm::vec2 mViewportSize;
	};
}