#include"Gui.h"

#include<imgui.h>
#include<backends/imgui_impl_opengl3.h>
#include<backends/imgui_impl_glfw.h>

#include"src/Application.h"

#include<ImGuiFileDialog.h>
#include"src/Scene/SceneContainer.h"

namespace ChoicePlus
{
	Gui::Gui()
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
			style.ScrollbarSize = 7.0f;
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		CustomDarkTheme();

		ImGui_ImplGlfw_InitForOpenGL(Application::Get()->GetWindow()->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	Gui::~Gui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Gui::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		FileDialogsModal();
	}

	void Gui::End()
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

	void Gui::FileDialogsModal()
	{
		if (ImGuiFileDialog::Instance()->Display("ImportModel", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove, ImVec2(800, 600)))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string dumpedmodelsrc = DumpModel(ImGuiFileDialog::Instance()->GetFilePathName(), "E:/Choice+/Choice+/assets/models/");

				SceneObject* object = new SceneObject();
				object->AddProperty<Model>(Load(dumpedmodelsrc));
				Transform* tranform = new Transform();
				object->AddProperty<Transform>(tranform);
				Application::Get()->GetEditor()->ActiveScene()->AddObject(object);
			}

			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGuiFileDialog::Instance()->Display("LoadScene", ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove, ImVec2(800, 600)))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::unique_ptr<SceneContainer> scenecontainer = std::make_unique<SceneContainer>();
				Application::Get()->GetEditor()->ActiveScene(scenecontainer->LoadContainedScene(ImGuiFileDialog::Instance()->GetFilePathName()));
			}

			ImGuiFileDialog::Instance()->Close();
		}
	}

	void Gui::CustomDarkTheme()
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
}