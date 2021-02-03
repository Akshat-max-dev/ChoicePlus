#pragma once
#include"c+pch.h"

namespace ChoicePlus
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind()const;
		void UnBind()const;

		const size_t GetCount()const { return mIndicesCount; }

		void VertexBuffer(const void* data, uint32_t size, std::string layout);
		void IndexBuffer(const void* data, uint32_t count);
	private:
		uint32_t mRendererId;
		uint32_t mVertexBuffer, mIndexBuffer;
		size_t mIndicesCount = 0;
	};
}