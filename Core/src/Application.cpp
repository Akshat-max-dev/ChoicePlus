#include "Application.h"

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"Input.h"

#include<imgui.h>
#include<backends/imgui_impl_opengl3.h>
#include<backends/imgui_impl_glfw.h>

#ifdef DEBUG
#include"Debug.h"
#endif

#include"OpenGL/Shader.h"

namespace ChoicePlus
{
	Application* Application::sInstance = nullptr;
	Application::Application()
	{
		sInstance = this;
		GUI_Init();
#ifdef DEBUG
		EnableOpenGLDebugging();
#endif
	}

	Application::~Application()
	{
		GUI_Destroy();
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
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			GUI_Begin();
			mEditor->Draw();
			GUI_End();

			glfwSwapBuffers(window);
		}
	}

	void Application::GUI_Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		io.FontDefault = io.Fonts->AddFontFromFileTTF("E:/Choice+/Choice+/assets/fonts/Roboto-Regular.ttf", 18.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.ScrollbarSize = 3.0f;
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
		GUI_SetDarkcolorTheme();

		ImGui_ImplGlfw_InitForOpenGL(mWindow->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void Application::GUI_Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Application::GUI_End()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void Application::GUI_Destroy()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Application::GUI_SetDarkcolorTheme()
	{
		auto& style = ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;

		style.TabRounding = 5.0f;
		style.FrameRounding = 5.0f;
		style.PopupRounding = 5.0f;
		style.ScrollbarRounding = 5.0f;
		style.GrabRounding = 5.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_CheckMark] = ImVec4{ 0.0f, 0.8f, 0.0f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
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