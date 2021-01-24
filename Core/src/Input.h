#pragma once
#include"c+pch.h"
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

namespace ChoicePlus
{
	class InputCallbacks
	{
	public:
		void Enable();
	private:
		static void window_close_callback(GLFWwindow* window);
		static void button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};

	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsButtonPressed(int button);
		static float GetMouseX() { return GetMousePosition().x; }
		static float GetMouseY() { return GetMousePosition().y; }
	private:
		static glm::vec2 GetMousePosition();
	};
}