#include "Application.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Input.h"

#include<imgui.h>
#include<backends/imgui_impl_opengl3.h>
#include<backends/imgui_impl_glfw.h>

namespace ChoicePlus
{
	Application* Application::sInstance = nullptr;
	Application::Application()
	{
		sInstance = this;
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
		GLFWwindow* window = mWindow->GetNativeWindow();
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	void Application::GUI_Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.ScrollbarSize = 3.0f;
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplGlfw_InitForOpenGL(mWindow->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void InputCallbacks::Enable()
	{
		GLFWwindow* window = Application::Get()->GetWindow()->GetNativeWindow();
		glfwSetWindowCloseCallback(window, window_close_callback);
	}

	void InputCallbacks::window_close_callback(GLFWwindow* window)
	{
		Application::Get()->Shutdown();
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
}