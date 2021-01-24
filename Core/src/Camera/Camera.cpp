#include"Camera.h"

#include<glm/gtc/matrix_transform.hpp>

namespace ChoicePlus
{
	Camera::Camera()
	{
		mProjection = glm::perspective(glm::radians(mFov), mAspectRatio, mNearClip, mFarClip);
		mViewProjection = mProjection * mView;
	}

	void Camera::Visible(const uint32_t w, const uint32_t h)
	{
		mAspectRatio = float(w) / float(h);
		RecalculateViewProjection();
	}

	void Camera::RecalculateViewProjection()
	{
		mProjection = glm::perspective(glm::radians(mFov), mAspectRatio, mNearClip, mFarClip);
		mViewProjection = mProjection * mView;
	}
	
}