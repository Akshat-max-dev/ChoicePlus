#include"EditorCamera.h"

#include<glm/gtc/matrix_transform.hpp>

#include"Input.h"

#include"src/Log.h"

namespace ChoicePlus
{
	EditorCamera::EditorCamera() :Camera()
	{
		mUp = { 0.0f, 1.0f, 0.0f };
		mOffset = { 0.0f, -1.0f, 5.0f };
		mFocus = { 0.0f, 0.0f, 0.0f };

		mView = glm::lookAt(mFocus + mOffset, mFocus, mUp);
		mPosition = mFocus + mOffset;

		mRight = glm::cross(mUp, glm::normalize(mOffset));
		mUp = glm::cross(glm::normalize(mOffset), mRight);
	}

	void EditorCamera::Update()
	{
		switch (mMovementType)
		{
		case MovementType::ROTATION:
		{
			float sensivity = 0.1f;
			glm::mat3 yaw = glm::rotate(glm::mat4(1.0f), glm::radians(mDeltaX * sensivity), glm::vec3(0, 1, 0));
			mOffset = yaw * mOffset;
			mUp = yaw * mUp;

			glm::vec3 forward = glm::normalize(-mOffset);
			mRight = glm::normalize(glm::cross(forward, mUp));

			glm::mat3 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(mDeltaY * sensivity), mRight);
			mOffset = pitch * mOffset;
			mUp = pitch * mUp;
			mView = glm::lookAt(mFocus + mOffset, mFocus, mUp);
			RecalculateViewProjection();
			break;
		}
		case MovementType::TRANSLATION:
		{
			float sensivity = 0.1f;
			mFocus += mRight * mDeltaX * sensivity;
			mFocus += mUp * mDeltaY * sensivity;
			mView = glm::lookAt(mFocus + mOffset, mFocus, mUp);
			RecalculateViewProjection();
			break;
		}
		}
	}

	void EditorCamera::OnMove(double xpos, double ypos)
	{
		mDeltaX = (float)xpos - mLastX;
		mDeltaY = mLastY - (float)ypos;
		mLastX = (float)xpos;
		mLastY = (float)ypos;
	}

	void EditorCamera::OnButtonDown(int button)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			mLastX = Input::GetMouseX();
			mLastY = Input::GetMouseY();
			mMovementType = Input::IsKeyPressed(GLFW_KEY_LEFT_ALT) ? MovementType::ROTATION : 
				Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) ? MovementType::TRANSLATION : MovementType::NONE;
		}
	}

	void EditorCamera::OnButtonUp(int button)
	{
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			mMovementType = MovementType::NONE;
		}
	}

	void EditorCamera::OnScroll(double yoffset)
	{
		mOffset -= (float)yoffset * mOffset * 0.05f;
		mView = glm::lookAt(mFocus + mOffset, mFocus, mUp);
		RecalculateViewProjection();
	}
}