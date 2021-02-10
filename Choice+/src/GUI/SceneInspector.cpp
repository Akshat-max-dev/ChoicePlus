#include"SceneInspector.h"

#include<imgui.h>
#include<imgui_internal.h>

#include"FontAwesome.h"

namespace ChoicePlus
{

	static void TransformUI(const std::string& label, glm::vec3& value, float resetValue)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 1, 1 });

		float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 ButtonSize = { lineheight + 3.0f, lineheight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 0.1f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });
		if (ImGui::Button("X", ButtonSize))
		{
			value.x = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 1.0f, 0.1f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.8f, 0.1f, 1.0f });
		if (ImGui::Button("Y", ButtonSize))
		{
			value.y = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.1f, 0.1f, 1.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });
		if (ImGui::Button("Z", ButtonSize))
		{
			value.z = resetValue;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.2f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T>
	void DrawProperty(T* prop) { static_assert(false); }

	template<>
	void DrawProperty<Transform>(Transform* transform)
	{
		if (ImGui::CollapsingHeader("Tranform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow))
		{
			TransformUI("Position", transform->Position, 0.0f);
			glm::vec3 rotation = glm::degrees(transform->Rotation);
			TransformUI("Rotation", rotation, 0.0f);
			transform->Rotation = glm::radians(rotation);
			TransformUI("Scale", transform->Scale, 1.0f);
		}
	}

	void SceneInspector::Draw(SceneObject* sceneobject)
	{
		ImGui::Begin(ICON_FK_INFO_CIRCLE);
		if (sceneobject)
		{
			auto transform = sceneobject->GetProperty<Transform>();
			if (transform)DrawProperty<Transform>(transform);
		}
		ImGui::End();
	}
}