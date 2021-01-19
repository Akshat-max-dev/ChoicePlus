#pragma once
#include"c+pch.h"
#include<GLFW/glfw3.h>
namespace ChoicePlus
{
	class Window
	{
	public:
		Window(const std::string& title, uint32_t w, uint32_t h);
		~Window();

		GLFWwindow* GetNativeWindow() { return mWindow; }
	private:
		std::string mTitle;
		uint32_t mWidth, mHeight;
		GLFWwindow* mWindow;
	private:
		void Create();
	};
}