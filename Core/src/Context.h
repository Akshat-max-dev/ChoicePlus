#pragma once
#include"c+pch.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>

namespace ChoicePlus
{
	class Context
	{
	public:
		void Create(GLFWwindow* hwnd)
		{
			glfwMakeContextCurrent(hwnd);
			cpassert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
			cpassert(GLVersion.major > 4 || (GLVersion.minor >= 5 || GLVersion.major > 4));
		}
	};
}