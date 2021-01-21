#include"Console.h"

namespace ChoicePlus
{
	ImVector<char*> Console::Items;
	Console::Console()
	{
		Clear();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;

		// "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
		Commands.push_back("HELP");
		Commands.push_back("HISTORY");
		Commands.push_back("CLEAR");
		Commands.push_back("CLASSIFY");
	}

	Console::~Console()
	{
		Clear();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	static char* Strdup(const char* s) 
	{ 
		size_t len = strlen(s) + 1; 
		void* buf = malloc(len); 
		cpassert(buf); 
		return (char*)memcpy(buf, (const void*)s, len);
	}

	void Console::Update(const char* fmt, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
	}

	void Console::Draw()
	{
		ImGui::Begin("Console", &mOpen, mFlags);
		if (ImGui::Button("Clear", ImVec2(45.0f, 25.0f)))Clear();
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y;
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_None);
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::Selectable("Clear")) Clear();
			ImGui::EndPopup();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
		for (int i = 0; i < Items.Size; i++)
		{
			const char* item = Items[i];
			if (!Filter.PassFilter(item))
				continue;

			std::string msg = item;
			ImVec4 color;
			bool has_color = false;
			if (strstr(item, "{e}")) 
			{ 
				color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); 
				has_color = true; 
			}
			else if (strstr(item, "{w}")) 
			{ 
				color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); 
				has_color = true; 
			}
			else if (strstr(item, "{i}"))
			{
				color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
				has_color = true;
			}
			if (has_color)
				ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(msg.substr(0, msg.find_last_of('{')).c_str());
			if (has_color)
				ImGui::PopStyleColor();
		}

		ImGui::PopStyleVar();

		if (1 && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		ImGui::EndChild();
		ImGui::End();
	}

	void Console::Clear()
	{
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
	}
}