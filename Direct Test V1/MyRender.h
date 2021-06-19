#pragma once

#include "AsimpMesh.h"
#include "Camera.h"
#include "D3D11_Framework.h"
#include "FireBall.h"
#include "Frustum.h"
#include "Model.h"
#include "ModelList.h"
#include "ParticleGenerator.h"
#include "Player.h"
#include "Timer.h"
#include "Labirint.h"

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	void initLight();
	bool Init();
	unsigned int lastUsedParticle = 0;
	
	
	bool Draw();
	void Close();

	void SetMoveLeftCam(bool moveLeftCam)
	{
		this->moveLeftCam = moveLeftCam;
	}

	void SetMoveRightCam(bool moveRightCam)
	{
		this->moveRightCam = moveRightCam;
	}

	void SetMoveForwardCam(bool moveForwardCam)
	{
		this->moveForwardCam = moveForwardCam;
	}

	void SetMoveBackCam(bool moveBackCam)
	{
		this->moveBackCam = moveBackCam;
	}

	void SetMoveUpCam(bool moveUpCam)
	{
		this->moveUpCam = moveUpCam;
	}

	void SetMoveDownCam(bool moveDownCam)
	{
		this->moveDownCam = moveDownCam;
	}

	void SetTurnLeftCam(bool turnLeftCam)
	{
		this->turnLeftCam = turnLeftCam;
	}

	void SetTurnRightCam(bool turnRightCam)
	{
		this->turnRightCam = turnRightCam;
	}

	void AddFireBallToRender(FireBall* fireball)
	{
		float speedParticle = 0.5;
		auto p = new ParticleGenerator(this);
		float rad = convertDegreeToRad(fireball->GetRotation().y);
		auto particleVel = XMFLOAT3(
			sinf(rad) * speedParticle * -1,
			0,
			cosf(rad) * speedParticle * -1
		);
		p->Init(fireBallParticleTexture, 300, particleVel, 0.11, 150);
		fireball->InitParticles(p);

		fireBalls.push_back(fireball);
		AddPointLight(fireball->light);
	}
	
	Player* GetPlayer();
	XMFLOAT3 GetWorldCords(int x, int y);

private:
	ModelList	modelList;
	
	Frustum		frustum;
	Timer		timer;

	Player* player;
	Labirint* labirint;
	Model* wallModel;
	Model* flourModel;
	Model* targetModel;

	ID3D11ShaderResourceView* fireBallParticleTexture;
	StaticMesh* mesh;
	list<FireBall*> fireBalls;
	ParticleGenerator* torchParticleGenerator;
	

	BitmapFont* font;
	Text*		textNumSphere;
	Text*		textCamCoord;

	bool		moveLeftCam;
	bool		moveRightCam;
	bool		moveForwardCam;
	bool		moveBackCam;
	bool		moveUpCam;
	bool		moveDownCam;
	bool		turnLeftCam;
	bool		turnRightCam;

	void handleCamMove();
	void updateAndDrawFireBalls(XMMATRIX viewMatrix);
	int drawLabirint(XMMATRIX viewMatrix);
	void drawPlayer(XMMATRIX viewMatrix);
};