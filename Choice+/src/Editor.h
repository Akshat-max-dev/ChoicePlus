#pragma once
#include"c+pch.h"
#include"Includes.h"

#include"GUI/Console.h"

#include"OpenGL/DeferredShading/DeferredPipeline.h"

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
		//void Update();
	private:
		std::unique_ptr<Console> mConsole = std::make_unique<Console>();
		std::unique_ptr<DeferredPipeline> mPipeline = std::make_unique<DeferredPipeline>();
		Scene* mActiveScene;
	};
}