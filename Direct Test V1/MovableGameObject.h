#pragma once
#include "GameObject.h"
class MovableGameObject :
    public GameObject
{
public:
	MovableGameObject();
	void TurnLeft(bool keydown);
	
	void TurnRight(bool keydown);
	void MoveForward(bool keydown);
	void MoveBack(bool keydown);
	void MoveLeft(bool keydown);
	void MoveRight(bool keydown);
	void MoveUp(bool keydown);
	void MoveDown(bool keydown);


	void SetSpeedTurn(float speedTurn)
	{
		this->speedTurn = speedTurn;
	}

	void SetSpeedMove(float speedMove)
	{
		this->speedMove = speedMove;
	}

	void SetMaxFrameTime(float time)
	{
		this->maxFrameTime = time;
	}

protected:
	void checkMaxSpeed(float& f);
	void checkMaxTurnSpeed(float& f);
	void checkNegate(float& f);

	float frameTime = 0.015;
	float speedTurn;
	float leftTurnSpeed, rightTurnSpeed;
	float leftMoveSpeed, rightMoveSpeed;
	float upMoveSpeed, downMoveSpeed;
	float forwardMoveSpeed, backMoveSpeed;
	float speedMove;
	float maxFrameTime;
};

