#pragma once
#include"c+pch.h"

#include"Camera.h"

namespace ChoicePlus
{
	class EditorCamera :public Camera
	{
	public:
		EditorCamera();
		void Update()override;
		void OnMove(double xpos, double ypos)override;
		void OnButtonDown(int button)override;
		void OnButtonUp(int button)override;
		void OnScroll(double yoffset);
	private:
		enum class MovementType
		{
			NONE = -1, TRANSLATION = 0, ROTATION = 1
		};
		MovementType mMovementType = MovementType::NONE;

		glm::vec3 mRight;
		glm::vec3 mUp = { 0.0f, 1.0f, 0.0f };
		glm::vec3 mOffset = { 0.0f, -1.0f, 5.0f };
		glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };

		float mDeltaX, mDeltaY, mLastX, mLastY;
	};
}