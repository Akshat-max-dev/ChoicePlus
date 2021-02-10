#include "Application.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Input.h"

#ifdef DEBUG
#include"Debug.h"
#endif

namespace ChoicePlus
{
	Application* Application::sInstance = nullptr;
	Application::Application()
	{
		sInstance = this;

		mWindow = std::make_unique<Window>("Choice+", 1600, 900);
		mGui = std::make_unique<Gui>();
		mEditor = std::make_unique<Editor>();

#ifdef DEBUG
		EnableOpenGLDebugging();
#endif

		InputCallbacks Callbacks;
		Callbacks.Enable();
	}

	Application::~Application()
	{
	}

	void Application::Shutdown()
	{
		glfwWindowShouldClose(mWindow->GetNativeWindow());
	}

	void Application::Run()
	{
		glfwSwapInterval(1);
		glEnable(GL_DEPTH_TEST);
		GLFWwindow* window = mWindow->GetNativeWindow();
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			mEditor->Update();

			mGui->Begin();
			mEditor->Draw();
			mGui->End();

			glfwSwapBuffers(window);
		}
	}

	void InputCallbacks::Enable()
	{
		GLFWwindow* window = Application::Get()->GetWindow()->GetNativeWindow();
		glfwSetWindowCloseCallback(window, window_close_callback);
		glfwSetCursorPosCallback(window, cursor_callback);
		glfwSetMouseButtonCallback(window, button_callback);
		glfwSetScrollCallback(window, scroll_callback);
	}

	void InputCallbacks::window_close_callback(GLFWwindow* window)
	{
		Application::Get()->Shutdown();
	}

	void InputCallbacks::button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)Application::Get()->GetEditor()->CurrentCamera()->OnButtonDown(button);
		else if (action == GLFW_RELEASE)Application::Get()->GetEditor()->CurrentCamera()->OnButtonUp(button);
	}

	void InputCallbacks::cursor_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Application::Get()->GetEditor()->CurrentCamera()->OnMove(xpos, ypos);
	}

	void InputCallbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Application::Get()->GetEditor()->CurrentCamera()->OnScroll(yoffset);
	}

	bool Input::IsKeyPressed(int key)
	{
		int action = glfwGetKey(Application::Get()->GetWindow()->GetNativeWindow(), key);
		return action == GLFW_PRESS;
	}

	bool Input::IsButtonPressed(int button)
	{
		int action = glfwGetMouseButton(Application::Get()->GetWindow()->GetNativeWindow(), button);
		return action == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition()
	{
		double xpos, ypos;
		glfwGetCursorPos(Application::Get()->GetWindow()->GetNativeWindow(), &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}
}