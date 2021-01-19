#pragma once
#include"c+pch.h"
#include<GLFW/glfw3.h>

namespace ChoicePlus
{
	class InputCallbacks
	{
	public:
		void Enable();
	private:
		static void window_close_callback(GLFWwindow* window);
	};

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsButtonPressed(int button);
	};
}