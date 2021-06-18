#pragma once
#include "stdafx.h"

#define SPEED_TURN 0.005f
#define SPEED_MOVE 0.01f
#define MIN_FRAME_TIME 0.03f


namespace D3D11Framework
{
//------------------------------------------------------------------

	class Camera
	{
	public:
		Camera();

		void Render(float time);
		void checkMaxSpeed(float& f);
		void checkNegate(float& f);

		void TurnLeft(bool keydown);
		void TurnRight(bool keydown);
		void MoveForward(bool keydown);
		void MoveBack(bool keydown);
		void MoveLeft(bool keydown);
		void MoveRight(bool keydown);
		void MoveUp(bool keydown);
		void MoveDown(bool keydown);

		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		XMFLOAT3 GetPosition()
		{
			return pos;
		}

		CXMMATRIX GetViewMatrix();

	private:
		XMMATRIX viewMatrix;
		XMFLOAT3 pos;
		XMFLOAT3 rot;
		

		float frameTime;
		float leftTurnSpeed, rightTurnSpeed;
		float leftMoveSpeed, rightMoveSpeed;
		float upMoveSpeed, downMoveSpeed;
		float forwardMoveSpeed, backMoveSpeed;
	};

//------------------------------------------------------------------
}