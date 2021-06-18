#include "stdafx.h"
#include "Camera.h"

#include "Log.h"

using namespace D3D11Framework;

Camera::Camera(): viewMatrix(), rightMoveSpeed(0),
 				  upMoveSpeed(0), downMoveSpeed(0), 
				  forwardMoveSpeed(0), backMoveSpeed(0)
{
	frameTime = 0.0f;
	leftTurnSpeed = 0.0f;
	rightTurnSpeed = 0.0f;
	leftMoveSpeed = 0.0f;

	pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void Camera::Render(float time)
{
	frameTime = time;
	float radians = rot.y * 0.0174532925f;
	float radiansRight = (rot.y + 90) * 0.0174532925f;
	float radiansZ = rot.z * 0.0174532925f;
	
	pos.x += sinf(radiansRight) * (rightMoveSpeed - leftMoveSpeed) + sinf(radians) * (forwardMoveSpeed - backMoveSpeed);
	pos.y += upMoveSpeed - downMoveSpeed;
	pos.z += cosf(radians) * (forwardMoveSpeed - backMoveSpeed) + cosf(radiansRight) * (rightMoveSpeed - leftMoveSpeed);
	//sLog->Debug("Xpos:%lf | Ypos:%lf | Zpos: %lf rad: %lf", pos.x, pos.y, pos.z, radians);

	
	float lAtx = sinf(radians) + pos.x;
	float lAty = pos.y - cosf(radiansZ);
	float lAtz = cosf(radians) + pos.z + sinf(radiansZ);
	//sLog->Debug("lAtXpos:%lf | lAtYpos:%lf | lAtZpos: %lf rad: %lf", lAtx, lAty, lAtz, radiansZ);
	XMVECTOR camPos = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
	XMVECTOR camLookAt = XMVectorSet( lAtx, lAty, lAtz, 0.0f );
	
	XMVECTOR camUp = XMVectorSet( 0.0f, cosf(radiansZ), sinf(radiansZ), 0.0f );

	viewMatrix = XMMatrixLookAtLH( camPos, camLookAt, camUp );
}

void Camera::checkMaxSpeed(float& f)
{
	if (f > (frameTime * MIN_FRAME_TIME))
		f = frameTime * MIN_FRAME_TIME;
	
}

void Camera::checkNegate(float& f)
{
	if (f < 0.0f)
		f = 0.0f;
}


void Camera::TurnLeft(bool keydown)
{
	if(keydown)
	{
		leftTurnSpeed += frameTime * SPEED_TURN;

		checkMaxSpeed(leftTurnSpeed);
	}
	else
	{
		leftTurnSpeed -= frameTime* SPEED_TURN/2;

		checkNegate(leftTurnSpeed);
	}

	rot.y -= leftTurnSpeed;
	if(rot.y < 0.0f)
		rot.y += 360.0f;
}

void Camera::TurnRight(bool keydown)
{
	if(keydown)
	{
		rightTurnSpeed += frameTime * SPEED_TURN;

		checkMaxSpeed(rightTurnSpeed);
	}
	else
	{
		rightTurnSpeed -= frameTime* SPEED_TURN/2;

		checkNegate(rightTurnSpeed);
	}

	rot.y += rightTurnSpeed;
	if(rot.y > 360.0f)
		rot.y -= 360.0f;
}

void Camera::MoveForward(bool keydown)
{
	if (keydown)
	{
		forwardMoveSpeed += frameTime * SPEED_MOVE;

		checkMaxSpeed(forwardMoveSpeed);
	}
	else
	{
		forwardMoveSpeed -= frameTime * SPEED_MOVE / 2;

		checkNegate(forwardMoveSpeed);
	}
}

void Camera::MoveBack(bool keydown)
{
	if (keydown)
	{
		backMoveSpeed += frameTime * SPEED_MOVE;
		checkMaxSpeed(backMoveSpeed);
	}
	else
	{
		backMoveSpeed -= frameTime * SPEED_MOVE / 2;
		checkNegate(backMoveSpeed);
	}
}

void Camera::MoveLeft(bool keydown)
{
	if (keydown)
	{
		leftMoveSpeed += frameTime * SPEED_MOVE;
		checkMaxSpeed(leftMoveSpeed);
	}
	else
	{
		leftMoveSpeed -= frameTime * SPEED_MOVE / 2;
		checkNegate(leftMoveSpeed);
	}
}

void Camera::MoveRight(bool keydown)
{
	if (keydown)
	{
		rightMoveSpeed += frameTime * SPEED_MOVE;
		checkMaxSpeed(rightMoveSpeed);
	}
	else
	{
		rightMoveSpeed -= frameTime * SPEED_MOVE/2;
		checkNegate(rightMoveSpeed);
	}
}

void Camera::MoveUp(bool keydown)
{
	if (keydown)
	{
		upMoveSpeed += frameTime * SPEED_MOVE;

		checkMaxSpeed(upMoveSpeed);
	}
	else
	{
		upMoveSpeed -= frameTime * SPEED_MOVE / 2;

		checkNegate(upMoveSpeed);
	}
}

void Camera::MoveDown(bool keydown)
{
	if (keydown)
	{
		downMoveSpeed += frameTime * SPEED_MOVE;

		checkMaxSpeed(downMoveSpeed);
	}
	else
	{
		downMoveSpeed -= frameTime * SPEED_MOVE / 2;

		checkNegate(downMoveSpeed);
	}
}

void Camera::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

CXMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}