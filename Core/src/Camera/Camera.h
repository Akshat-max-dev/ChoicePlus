#pragma once
#include"c+pch.h"

#include<glm/glm.hpp>

namespace ChoicePlus
{
	class Camera
	{
	public:
		Camera();
		virtual void Update() {}
		void Visible(const uint32_t w, const uint32_t h);

		std::pair<glm::mat4, glm::vec3> CameraData() { return { mViewProjection, mPosition }; }

		virtual void OnMove(double xpos, double ypos) {}
		virtual void OnButtonDown(int button) {}
		virtual void OnButtonUp(int button) {}
		virtual void OnScroll(double yoffset) {}
	protected:
		void RecalculateViewProjection();
	protected:
		glm::vec3 mPosition;

		glm::mat4 mProjection;
		glm::mat4 mView = glm::mat4(1.0f);
		glm::mat4 mViewProjection;

		float mAspectRatio = 0.0f;
		float mFov = 60.0f;
		float mFarClip = 1000.0f;
		float mNearClip = 0.1f;
	};
}