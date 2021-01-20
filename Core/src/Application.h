#pragma once
#include"c+pch.h"
#include"Window.h"
#include"Editor.h"

namespace ChoicePlus
{
	class Editor;
}

namespace ChoicePlus
{
	class Application
	{
	public:
		Application();
		~Application();
		void Shutdown();
		void Run();

		static Application* Get() { return sInstance; }
		std::unique_ptr<Window>& GetWindow() { return mWindow; }
	private:
		static Application* sInstance;
		void GUI_Init();
		void GUI_Begin();
		void GUI_End();
		void GUI_Destroy();
		void GUI_SetDarkcolorTheme();
	private:
		std::unique_ptr<Editor> mEditor = std::make_unique<Editor>();
		std::unique_ptr<Window> mWindow = std::make_unique<Window>("Choice+", 1600, 900);
	};
}