#pragma once
#include"c+pch.h"

#include<glad/glad.h>

namespace ChoicePlus
{
	enum class BufferType
	{
		VERTEX = 0, INDEX = 1
	};

	template<BufferType T>
	class Buffer{};

	template<>
	class Buffer<BufferType::VERTEX>
	{
	public:
		Buffer(void* data, uint32_t size);
		void Bind()const { glBindBuffer(GL_ARRAY_BUFFER, mRendererId); }
		void UnBind()const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	private:
		uint32_t mRendererId;
	};

	inline Buffer<BufferType::VERTEX>::Buffer(void* data, uint32_t size)
	{
		glCreateBuffers(1, &mRendererId);
		glBindBuffer(GL_ARRAY_BUFFER, mRendererId);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	template<>
	class Buffer<BufferType::INDEX>
	{
	public:
		Buffer(void* data, uint32_t count);
		void Bind()const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId); }
		void UnBind()const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
		const uint32_t& GetCount()const { return mCount; }
	private:
		uint32_t mRendererId, mCount;
	};

	inline Buffer<BufferType::INDEX>::Buffer(void* data, uint32_t count):mCount(count)
	{
		glCreateBuffers(1, &mRendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}
}