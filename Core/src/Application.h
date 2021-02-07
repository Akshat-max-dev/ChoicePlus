#pragma once
#include"c+pch.h"
#include"Window.h"
#include"Editor.h"
#include"GUI/Gui.h"

namespace ChoicePlus
{
	class Editor;
	class Gui;
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
		std::unique_ptr<Editor>& GetEditor() { return mEditor; }
	private:
		static Application* sInstance;
	private:
		std::unique_ptr<Window> mWindow;
		std::unique_ptr<Gui> mGui;
		std::unique_ptr<Editor> mEditor;
	};
}