#pragma once
#include"c+pch.h"

namespace ChoicePlus
{
	class FileDialogs
	{
	public:
		static std::optional<std::string> OpenFile(const char* filter);
		static std::optional<std::string> SaveFile(const char* filter);
	};
}