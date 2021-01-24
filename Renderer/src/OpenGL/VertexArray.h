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

		const std::optional<std::shared_ptr<Buffer<BufferType::INDEX>>>& GetIndexBuffer()const { return mIndexBuffer; }
		
		void Setup(const std::shared_ptr<Buffer<BufferType::VERTEX>>& vertexBuffer, 
			const std::string& layout);
	
		void SetIndexBuffer(const std::optional<std::shared_ptr<Buffer<BufferType::INDEX>>>& indexBuffer);

	private:
		uint32_t mRendererId;
		std::shared_ptr<Buffer<BufferType::VERTEX>> mVertexBuffer;
		std::optional<std::shared_ptr<Buffer<BufferType::INDEX>>> mIndexBuffer;
	};
}