#pragma once
#include"c+pch.h"
#include"Includes.h"

#include"GUI/Console.h"

namespace ChoicePlus
{
	class Console;
}

namespace ChoicePlus
{
	class Editor
	{
	public:
		Editor();
		~Editor();
		void Draw();
	private:
		std::unique_ptr<Console> mConsole = std::make_unique<Console>();
	};
}