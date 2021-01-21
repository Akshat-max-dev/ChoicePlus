#pragma once
#include"c+pch.h"

#include"Buffer.h"

namespace ChoicePlus
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void Bind()const;
		void UnBind()const;
		
		void Setup(const std::shared_ptr<Buffer<BufferType::VERTEX>>& vertexBuffer, 
			const std::optional<std::shared_ptr<Buffer<BufferType::INDEX>>>& indexBuffer, const std::string& layout);
	private:
		uint32_t mRendererId;
		std::shared_ptr<Buffer<BufferType::VERTEX>> mVertexBuffer;
		std::shared_ptr<Buffer<BufferType::INDEX>> mIndexBuffer;
	};
}