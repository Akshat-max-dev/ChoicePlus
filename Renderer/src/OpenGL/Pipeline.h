#pragma once
#include"c+pch.h"

#include"Shader.h"
#include"FrameBuffer.h"
#include"src/Scene/Scene.h"

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
	protected:
		virtual void Init();
		virtual void Visible(const uint32_t w, const uint32_t h);
		virtual void Draw(const Scene* scene);
		std::pair<std::unique_ptr<CaptureSkybox>, std::unique_ptr<Shader>> mCaptureSkyboxPass;
	};
}