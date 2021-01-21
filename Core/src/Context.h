#pragma once
#include"c+pch.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Log.h"
namespace ChoicePlus
{
	class Context
	{
	public:
		void Create(GLFWwindow* hwnd)
		{
			glfwMakeContextCurrent(hwnd);
			cpassert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
			cpassert((GLVersion.major > 4 || (GLVersion.minor >= 5 && GLVersion.major == 4)));

			CONSOLE("---------------------------------------{i}");
			CONSOLE("OpenGL Info{i}");
			CONSOLE("---------------------------------------{i}");

			std::string vendor = "Vendor : ";
			vendor += (const char*)glGetString(GL_VENDOR);
			vendor.append("{i}");
			CONSOLE(vendor.c_str());

			std::string renderer = "Renderer : ";
			renderer += (const char*)glGetString(GL_RENDERER);
			renderer.append("{i}");
			CONSOLE(renderer.c_str());

			std::string version = "Version : ";
			version += (const char*)glGetString(GL_VERSION);
			version.append("{i}");
			CONSOLE(version.c_str());
			CONSOLE("---------------------------------------{i}");
		}
	};
}