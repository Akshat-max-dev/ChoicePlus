#pragma once
#include"c+pch.h"

#include"OpenGL/Pipeline.h"
#include"OpenGL/Shader.h"
#include"OpenGL/FrameBuffer.h"
#include"src/Scene/Scene.h"

#include<glm/glm.hpp>

namespace ChoicePlus
{
	class DeferredGeometryCapture : public FrameBuffer
	{
	public:
		DeferredGeometryCapture();
		~DeferredGeometryCapture();

		void BindGBuffer(const glm::uvec4& slots);
	private:
		void Invalidate()override;
	private:
		std::vector<uint32_t> mGBuffer;
	};

	class DeferredLightingCapture : public FrameBuffer
	{
	public:
		DeferredLightingCapture();
		~DeferredLightingCapture();

		void BindFinalResult(const uint32_t slot)const;
		const uint32_t& GetFinalResultId()const { return mFinalResult; }
	private:
		void Invalidate()override;
	private:
		uint32_t mFinalResult;
	};

	class DeferredPipeline : public Pipeline
	{
	public:
		void Init()override;
		void Visible(const uint32_t w, const uint32_t h)override;
		void Draw(const Scene* scene)override; //TO COMPLETE PARAMETER
	private:
		std::pair<std::unique_ptr<DeferredGeometryCapture>, std::unique_ptr<Shader>> mGeometryPass;
		std::pair<std::unique_ptr<DeferredLightingCapture>, std::unique_ptr<Shader>> mLightingPass;
	};
}