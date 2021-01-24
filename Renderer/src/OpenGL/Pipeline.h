#pragma once
#include"c+pch.h"

#include"Shader.h"
#include"FrameBuffer.h"
#include"src/Scene/Scene.h"

#include<glm/glm.hpp>

namespace ChoicePlus
{
	class CaptureSkybox :public FrameBuffer
	{
	public:
		CaptureSkybox();
		~CaptureSkybox();

		void BindCapture(const uint32_t slot)const;
	private:
		void Invalidate()override;
	private:
		uint32_t mCaptureId;
	};

	class Pipeline
	{
	public:
		virtual void Init();
		virtual void Visible(const uint32_t w, const uint32_t h);
		virtual void Update(Scene* with, std::pair<glm::mat4, glm::vec3>& to);
		virtual const uint32_t FinalResult()const { return 0; }
	protected:
		std::pair<std::unique_ptr<CaptureSkybox>, std::unique_ptr<Shader>> mCaptureSkyboxPass;
	};
}