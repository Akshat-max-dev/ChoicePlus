#include"FrameBuffer.h"

#include<glad/glad.h>
#include"src/Log.h"

namespace ChoicePlus
{
	FrameBuffer::~FrameBuffer()
	{
		if (mRendererId)Destroy();
	}

	void FrameBuffer::Destroy()
	{
		if (mRendererId)
		{
			glDeleteFramebuffers(1, &mRendererId);
		}
	}

	void FrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);
		glViewport(0, 0, mWidth, mHeight);
	}

	void FrameBuffer::UnBind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::Visible(const uint32_t w, const uint32_t h)
	{
		mWidth = w;
		mHeight = h;
		Invalidate();
	}
}