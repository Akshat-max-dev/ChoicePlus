#include"DeferredPipeline.h"

#include<glad/glad.h>
#include"src/Log.h"

namespace ChoicePlus
{
	void DeferredPipeline::Init()
	{
		mGeometryPass = { std::make_unique<DeferredGeometryCapture>(), std::make_unique<Shader>("E:/Choice+/Renderer/assets/Shaders/DeferredGeometryPass.glsl") };
		Pipeline::Init();
		mLightingPass = { std::make_unique<DeferredLightingCapture>(), std::make_unique<Shader>("E:/Choice+/Renderer/assets/Shaders/DeferredLightingPass.glsl") };
	}

	void DeferredPipeline::Visible(const uint32_t w, const uint32_t h)
	{
		Pipeline::Visible(w, h);
		mGeometryPass.first->Visible(w, h);
		mLightingPass.first->Visible(w, h);
	}

	void DeferredPipeline::Update(Scene* with, std::pair<glm::mat4, glm::vec3>& to)
	{
		Pipeline::Update(with, to);
		mGeometryPass.first->Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto &object : with->GetSceneObjects())
		{
			auto model = object.GetProperty<Model>();
			if (model)
			{
				for (auto& mesh : model->GetMeshes())
				{
					mGeometryPass.second->Bind();
					if (model->GetMaterials()[mesh.second]->mDiffuseMap)
						model->GetMaterials()[mesh.second]->mDiffuseMap->Bind(0);
					mGeometryPass.second->Int("gMaterial.Diffuse", 0);
					if (model->GetMaterials()[mesh.second]->mNormalMap)
						model->GetMaterials()[mesh.second]->mNormalMap->Bind(1);
					mGeometryPass.second->Int("gMaterial.Normal", 1);

					mGeometryPass.second->Mat4("uViewProjection", to.first);
					mGeometryPass.second->Mat4("uTransform", glm::mat4(1.0));
					mesh.first->Bind();
					uint32_t count = mesh.first->GetIndexBuffer().value()->GetCount();
					glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
				}
			}
		}
		mGeometryPass.first->UnBind();
		
		mLightingPass.first->Bind();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		mLightingPass.second->Bind();
		mGeometryPass.first->BindGBuffer({ 0, 1, 2, 3 });
		mLightingPass.second->Int("gBuffer.AlbedoS", 2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		mLightingPass.first->UnBind();
	}

	DeferredGeometryCapture::DeferredGeometryCapture() :FrameBuffer()
	{
		Invalidate();
	}

	DeferredGeometryCapture::~DeferredGeometryCapture()
	{
		FrameBuffer::Destroy();
		glDeleteTextures(mGBuffer.size(), &mGBuffer[0]);
	}

	void DeferredGeometryCapture::BindGBuffer(glm::uvec4 slots)
	{
		glActiveTexture(GL_TEXTURE0 + slots.x);
		glBindTexture(GL_TEXTURE_2D, mGBuffer[0]);
		glActiveTexture(GL_TEXTURE0 + slots.y);
		glBindTexture(GL_TEXTURE_2D, mGBuffer[1]);
		glActiveTexture(GL_TEXTURE0 + slots.z);
		glBindTexture(GL_TEXTURE_2D, mGBuffer[2]);
		glActiveTexture(GL_TEXTURE0 + slots.w);
		glBindTexture(GL_TEXTURE_2D, mGBuffer[3]);
	}

	void DeferredGeometryCapture::Invalidate()
	{
		if (mRendererId)
		{
			FrameBuffer::Destroy();
			if (mGBuffer.size() != 0)glDeleteTextures(mGBuffer.size(), &mGBuffer[0]);
		}

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		uint32_t Id;
		glCreateTextures(GL_TEXTURE_2D, 1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Id, 0);

		mGBuffer.push_back(Id);

		glCreateTextures(GL_TEXTURE_2D, 1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Id, 0);

		mGBuffer.push_back(Id);

		glCreateTextures(GL_TEXTURE_2D, 1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, Id, 0);

		mGBuffer.push_back(Id);

		glCreateTextures(GL_TEXTURE_2D, 1, &Id);
		glBindTexture(GL_TEXTURE_2D, Id);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Id, 0);

		mGBuffer.push_back(Id);

		uint32_t Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, Attachments);

		if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
		{
			#ifdef DEBUG
				CONSOLE("Framebuffer Incomplete{e}");
				cpassert(0);
			#endif
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	DeferredLightingCapture::DeferredLightingCapture() :FrameBuffer()
	{
		Invalidate();
	}

	DeferredLightingCapture::~DeferredLightingCapture()
	{
		FrameBuffer::Destroy();
		glDeleteTextures(1, &mFinalResult);
	}

	void DeferredLightingCapture::BindFinalResult(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, mFinalResult);
	}

	void DeferredLightingCapture::Invalidate()
	{
		if (mRendererId)
		{
			FrameBuffer::Destroy();
			glDeleteTextures(1, &mFinalResult);
		}

		glCreateFramebuffers(1, &mRendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererId);

		glCreateTextures(GL_TEXTURE_2D, 1, &mFinalResult);
		glBindTexture(GL_TEXTURE_2D, mFinalResult);

		glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, mWidth, mHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mFinalResult, 0);

		if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
		{
			#ifdef DEBUG
				CONSOLE("Framebuffer Incomplete{e}");
				cpassert(0);
			#endif
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}