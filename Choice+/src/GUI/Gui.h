#pragma once
#include"c+pch.h"

namespace ChoicePlus
{
	class Gui
	{
	public:
		Gui();
		~Gui();
		void Begin();
		void End();
	private:
		void CustomDarkTheme();
	};
}