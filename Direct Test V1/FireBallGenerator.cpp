#include "FireBallGenerator.h"

Timer* FireBallGenerator::timer;
StaticMesh* FireBallGenerator::model;

FireBall* FireBallGenerator::Generate(XMFLOAT3 pos, XMFLOAT3 rot, int lifeTime)
{
	auto result = new FireBall();
	result->SetPosition(pos.x, pos.y, pos.z);
	result->InitPointLight();
	result->SetRotation(rot.x, rot.y, rot.z);
	result->lifeTime = lifeTime;
	result->model = model;
	result->SetSpeedMove(0.1);
	result->SetMaxFrameTime(0.012);

	return result;
}
