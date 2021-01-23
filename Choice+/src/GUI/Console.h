#pragma once
#ifndef CONSOLE_H_
#define CONSOLE_H_
#include"c+pch.h"

#include<imgui.h>

namespace ChoicePlus
{
	class Console
	{
	public:
		Console();
		~Console();
		static void Update(const char* fmt, ...);
		void Draw();
	private:
		void Clear();
	private:
		bool mOpen = true;

		char                  InputBuf[256];
		static ImVector<char*>Items;
		ImVector<const char*> Commands;
		ImVector<char*>       History;
		int                   HistoryPos;
		ImGuiTextFilter       Filter;

		ImGuiWindowFlags mFlags = ImGuiWindowFlags_NoNavInputs;
	};
}
#endif