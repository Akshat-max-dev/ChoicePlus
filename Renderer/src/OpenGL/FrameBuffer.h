#pragma once
#include"c+pch.h"

namespace ChoicePlus
{
	class FrameBuffer
	{
	public:
		~FrameBuffer();

		void Bind()const;
		void UnBind()const;

		void Visible(const uint32_t w, const uint32_t h);
	protected:
		void Destroy();
		virtual void Invalidate() = 0;
		uint32_t mRendererId = -1;
		uint32_t mWidth = 1280;
		uint32_t mHeight = 720;
	};
}