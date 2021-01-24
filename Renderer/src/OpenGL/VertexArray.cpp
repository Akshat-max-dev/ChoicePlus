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

	void VertexArray::Setup(const std::shared_ptr<Buffer<BufferType::VERTEX>>& vertexBuffer, 
		const std::string& layout)
	{
		cpassert(vertexBuffer);
		glBindVertexArray(mRendererId);
		vertexBuffer->Bind();

		std::vector<int> Counts;
		GLsizei strides = 0;
		for (auto c : layout)
		{
			if (int(c) >= 49 && int(c) <= 57)
			{
				Counts.push_back(int(c) - 48);
				strides += (int(c) - 48) * sizeof(float);
			}
		}

		size_t offset = 0;
		for (uint32_t i = 0; i < Counts.size(); i++)
		{
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, Counts[i], GL_FLOAT, GL_FALSE, strides, (const void*)offset);
			offset += Counts[i] * sizeof(float);
		}

		mVertexBuffer = vertexBuffer;
	}

	void VertexArray::SetIndexBuffer(const std::optional<std::shared_ptr<Buffer<BufferType::INDEX>>>& indexBuffer)
	{
		if (indexBuffer.has_value())
		{
			indexBuffer.value()->Bind();
			mIndexBuffer = indexBuffer;
		}
		else mIndexBuffer = std::nullopt;
	}
}