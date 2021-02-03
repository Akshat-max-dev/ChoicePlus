#include "VertexArray.h"

#include<glad/glad.h>

namespace ChoicePlus
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &mRendererId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &mVertexBuffer);
		glDeleteBuffers(1, &mIndexBuffer);
		glDeleteVertexArrays(1, &mRendererId);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(mRendererId);
	}

	void VertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void VertexArray::VertexBuffer(const void* data, uint32_t size, std::string layout)
	{
		cpassert(data);
		glBindVertexArray(mRendererId);
		
		glCreateBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

		std::vector<int> Counts;
		uint32_t strides = 0;
		for (auto c : layout)
		{
			if (int(c) >= 49 && int(c) <= 57)
			{
				Counts.push_back(int(c) - 48);
				strides += (int(c) - 48) * sizeof(float);
			}
		}

		uint32_t offset = 0;
		for (uint32_t i = 0; i < Counts.size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, Counts[i], GL_FLOAT, GL_FALSE, strides, (const void*)offset);
			offset += Counts[i] * sizeof(float);
		}

		glBindVertexArray(0);
	}

	void VertexArray::IndexBuffer(const void* data, uint32_t count)
	{
		mIndicesCount = count;
		glCreateBuffers(1, &mIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
	}
}