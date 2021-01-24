#include"Pipeline.h"

#include<glad/glad.h>
#include"src/Log.h"

namespace ChoicePlus
{
	static uint32_t sMaxFramebufferSize = 8192;

	CaptureSkybox::CaptureSkybox() :FrameBuffer()
	{
		Invalidate();
	}

	CaptureSkybox::~CaptureSkybox()
	{
		FrameBuffer::Destroy();
		glDeleteTextures(1, &mCaptureId);
	}

	void CaptureSkybox::BindCapture(const uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mCaptureId);
	}

	void CaptureSkybox::Invalidate()
	{
		if (mRendererId)
		{
			FrameBuffer::Destroy();
			glDeleteTextures(1, &mRendererId);
		}

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &mCaptureId);
		glBindTexture(GL_TEXTURE_2D, mCaptureId);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mCaptureId, 0);

		if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
		{
			#ifdef DEBUG
				CONSOLE("Framebuffer Incomplete{e}");
				cpassert(0);
			#endif
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Pipeline::Init()
	{
		mCaptureSkyboxPass = { std::make_unique<CaptureSkybox>(), std::make_unique<Shader>("E:/Choice+/Renderer/assets/Shaders/Skybox.glsl") };
	}

	void Pipeline::Visible(const uint32_t w, const uint32_t h)
	{
		if (w == 0 || h == 0 || w > sMaxFramebufferSize || h > sMaxFramebufferSize)
		{
			std::string msg = "Attempt to Set Visible Region to " + std::to_string(w);
			msg += " ";
			msg += std::to_string(h);
			msg.append("{e}");
			CONSOLE(msg.c_str());
			return;
		}
		mCaptureSkyboxPass.first->Visible(w, h);
	}

	void Pipeline::Update(Scene* with, std::pair<glm::mat4, glm::vec3>& to)
	{
		mCaptureSkyboxPass.first->Bind();
		mCaptureSkyboxPass.second->Bind();
		//TODO DRAW Skybox Here
		mCaptureSkyboxPass.second->UnBind();
		mCaptureSkyboxPass.first->UnBind();
	}
}