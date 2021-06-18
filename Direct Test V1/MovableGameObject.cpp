#include "MovableGameObject.h"



void MovableGameObject::checkMaxSpeed(float& f)
{
	if (f > (frameTime * maxFrameTime))
		f = frameTime * maxFrameTime;

}

void MovableGameObject::checkMaxTurnSpeed(float& f)
{
	if (f > 1)
		f = 1;
}

void MovableGameObject::checkNegate(float& f)
{
	if (f < 0.0f)
		f = 0.0f;
}


MovableGameObject::MovableGameObject(): maxFrameTime(MIN_FRAME_TIME),speedTurn(SPEED_TURN), leftTurnSpeed(0), rightTurnSpeed(0), leftMoveSpeed(0),
                                        rightMoveSpeed(0),
                                        upMoveSpeed(0),
                                        downMoveSpeed(0),
                                        forwardMoveSpeed(0),
                                        backMoveSpeed(0),
                                        speedMove(SPEED_MOVE)
{
}

void MovableGameObject::TurnLeft(bool keydown)
{
	if (keydown)
	{
		leftTurnSpeed += frameTime * speedTurn;

		checkMaxTurnSpeed(leftTurnSpeed);
	}
	else
	{
		leftTurnSpeed -= frameTime * speedTurn / 2;

		checkNegate(leftTurnSpeed);
	}

	rot.y -= leftTurnSpeed;
	if (rot.y < 0.0f)
		rot.y += 360.0f;
}

void MovableGameObject::TurnRight(bool keydown)
{
	if (keydown)
	{
		rightTurnSpeed += frameTime * speedTurn;;

		checkMaxTurnSpeed(rightTurnSpeed);
	}
	else
	{
		rightTurnSpeed -= frameTime * speedTurn / 2;

		checkNegate(rightTurnSpeed);
	}

	rot.y += rightTurnSpeed;
	if (rot.y > 360.0f)
		rot.y -= 360.0f;
}

void MovableGameObject::MoveForward(bool keydown)
{
	if (keydown)
	{
		forwardMoveSpeed += frameTime * speedMove;;

		checkMaxSpeed(forwardMoveSpeed);
	}
	else
	{
		forwardMoveSpeed -= frameTime * speedMove / 2;

		checkNegate(forwardMoveSpeed);
	}
}

void MovableGameObject::MoveBack(bool keydown)
{
	if (keydown)
	{
		backMoveSpeed += frameTime * speedMove;
		checkMaxSpeed(backMoveSpeed);
	}
	else
	{
		backMoveSpeed -= frameTime * speedMove / 2;
		checkNegate(backMoveSpeed);
	}
}

void MovableGameObject::MoveLeft(bool keydown)
{
	if (keydown)
	{
		leftMoveSpeed += frameTime * speedMove;
		checkMaxSpeed(leftMoveSpeed);
	}
	else
	{
		leftMoveSpeed -= frameTime * speedMove / 2;
		checkNegate(leftMoveSpeed);
	}
}

void MovableGameObject::MoveRight(bool keydown)
{
	if (keydown)
	{
		rightMoveSpeed += frameTime * speedMove;
		checkMaxSpeed(rightMoveSpeed);
	}
	else
	{
		rightMoveSpeed -= frameTime * speedMove / 2;
		checkNegate(rightMoveSpeed);
	}
}

void MovableGameObject::MoveUp(bool keydown)
{
	if (keydown)
	{
		upMoveSpeed += frameTime * speedMove;;

		checkMaxSpeed(upMoveSpeed);
	}
	else
	{
		upMoveSpeed -= frameTime * speedMove / 2;

		checkNegate(upMoveSpeed);
	}
}

void MovableGameObject::MoveDown(bool keydown)
{
	if (keydown)
	{
		downMoveSpeed += frameTime * speedMove;;

		checkMaxSpeed(downMoveSpeed);
	}
	else
	{
		downMoveSpeed -= frameTime * speedMove / 2;

		checkNegate(downMoveSpeed);
	}
}