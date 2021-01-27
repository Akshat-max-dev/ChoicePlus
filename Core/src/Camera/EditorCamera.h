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
		glm::vec3 mUp;
		glm::vec3 mOffset;
		glm::vec3 mFocus;

		float mDeltaX, mDeltaY, mLastX, mLastY;
	};
}