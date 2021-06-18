#pragma once
#include "FireBall.h"
#include "Timer.h"
#include "StaticMesh.h"

class FireBallGenerator
{
public:
	static Timer* timer;
	static StaticMesh* model;
	static FireBall* Generate(XMFLOAT3 pos, XMFLOAT3 rot, int lifeTime);
	
};



