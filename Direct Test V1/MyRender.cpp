#pragma once

#include "MyRender.h"

#include "FireBallGenerator.h"
#include "ParticleGenerator.h"
#include "Util.h"

#define SimpleXMVector(r) XMVectorSet(r, r, r, 0);


MyRender::MyRender(): frustum(), timer(), player(nullptr),
                      moveForwardCam(false), moveBackCam(false),
                      moveUpCam(false), moveDownCam(false),
                      turnLeftCam(false), turnRightCam(false)
{
	mesh = nullptr;
	labirint = nullptr;
	font = nullptr;
	textNumSphere = nullptr;
	moveLeftCam = moveRightCam = false;
}

void MyRender::initLight()
{
	DirectionalLight m_DirLight;
	m_DirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	m_DirectionalLights[0] = m_DirLight;
	numDirLight = 0;

	PointLight m_PointLight;
	m_PointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	m_PointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_PointLight.diffuse = XMFLOAT4(0.5f, 0.2f, 0.2f, 1.0f);
	m_PointLight.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_PointLight.att = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//m_PointLight.color = XMFLOAT3(0.5f, 0.1f, 0.0f);
	m_PointLight.range = 25.0f;
	
	AddPointLight(*player->GetTorchLight());
	//m_PointLights.push_back(m_PointLight);
	

	SpotLight m_SpotLight;
	m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_SpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_SpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_SpotLight.spot = 12.0f;
	m_SpotLight.range = 10000.0f;
	m_SpotLights[0] = m_SpotLight;
	numSpotLight = 0;
}

bool MyRender::Init()
{
	mesh = new MeshMS3D(this);
	if (!mesh->Init(w("sphere.ms3d")))
		return false;

	FireBallGenerator::model = mesh;
	FireBallGenerator::timer = &timer;

	labirint = new Labirint();
	wallModel = new Model(this);
	wallModel->Init("wall.obj");

	
	labirint->Init(wallModel);
	if (labirint->LoadFromFile("1.txt"))
		return false;
	//labirint = new Model(this);
	//labirint->Init(("maze.obj"));

	player = new Player();
	player->InitModel(this, "grim.obj");
	player->SetSpeedTurn(2);
	player->SetSpeedMove(2);
	player->SetMaxFrameTime(1.5);
	player->SetPosition(labirint->spawnPlayer.x, labirint->spawnPlayer.y, labirint->spawnPlayer.z);
	
	font = new BitmapFont(this);
	if(!font->Init("font.fnt"))
		return false;

	textNumSphere = new Text(this, font);
	if(!textNumSphere->Init(L"", false, 20))
		return false;

	textCamCoord = new Text(this, font);
	if (!textCamCoord->Init(L"", false, 30))
		return false;

	flourModel = new Model(this);
	
	if (!flourModel->Init("flour.obj"))
		return false;

	if(!modelList.Init(10))
		return false;

	if(!timer.Init())
		return false;

	cam.SetPosition(player->GetPosition().x, 14, player->GetPosition().z-14.0f);
	cam.SetRotation(0, 0, 0);

	initLight();

	torchParticleGenerator = new ParticleGenerator(this);
	if (!torchParticleGenerator->Init(w("particle.png"), 200, XMFLOAT3(0, 0.01f, 0), 0.2))
		return false;
	torchParticleGenerator->model = mesh;
	return true;
}

void MyRender::handleCamMove()
{
	cam.TurnLeft(turnLeftCam);
	cam.TurnRight(turnRightCam);
	cam.MoveDown(moveDownCam);
	cam.MoveUp(moveUpCam);
	cam.MoveBack(moveBackCam);
	cam.MoveForward(moveForwardCam);
	cam.MoveLeft(moveLeftCam);
	cam.MoveRight(moveRightCam);
}

void MyRender::updateAndDrawFireBalls(XMMATRIX viewMatrix)
{
	if(fireBalls.size() > 0)
	{
		list<FireBall*> delFireballs;
		//list<int> delIndFireballs;
		list<PointLightCont*>::iterator it = m_PointLights.begin();
		int i = 1;
		std::advance(it, 1);
		for (FireBall* fireBall : fireBalls)
		{
			
			fireBall->UpdatePos(timer.GetFrameTime());
			it._Ptr->_Myval->light.position = fireBall->light.position;
			fireBall->Draw(viewMatrix);
			
			if (fireBall->lifeTime < 0)
			{
				delFireballs.push_back(fireBall);
				//delIndFireballs.push_back(i);
				it = m_PointLights.erase(it);
			}
			else
			{
				std::advance(it, 1);
			}
			i++;
		}
		//auto iter = delIndFireballs.begin();
		for (FireBall* fireBall : delFireballs)
		{
			fireBalls.remove(fireBall);
			//std::advance(it, iter._Ptr->_Myval); // <-- advance итерирует переданный итератор на k позиций
			
			// <--- Вернет итератор на k+1 элемент, перед it нет *
			delete fireBall;
		}
	}
}

bool MyRender::Draw()
{
	timer.Frame();
	handleCamMove();
	auto frameTime = timer.GetFrameTime();
	cam.Render(frameTime);
	

	XMMATRIX viewMatrix = cam.GetViewMatrix();
	frustum.ConstructFrustum(1000, m_Projection, viewMatrix);

	flourModel->Draw(viewMatrix);
	
	int modelCount = modelList.GetModelCount();
	int renderCount = 0;

	for (int i = 0; i < modelCount; i++)
	{
		float x, y, z;
		modelList.GetData(i, x, y, z);
		bool renderModel = frustum.CheckSphere(x, y, z, 1);
		if(renderModel)
		{
			mesh->Identity();
			mesh->Translate(x, y, z);
			mesh->Draw(viewMatrix);

			renderCount++;
		}
	}
	//labirint->Identity();
	//labirint->Scale(2, 2, 2);
	labirint->Draw(viewMatrix);

	player->Draw(viewMatrix);
	updateAndDrawFireBalls(viewMatrix);

	
	TurnOnAlphaBlending();

	
	torchParticleGenerator->Update(frameTime, player->GetTorchLight()->position, 2);
	torchParticleGenerator->Draw(viewMatrix);
	
	TurnZBufferOff();
	std::wstring t = L"Сфер на экране: " + intToStr(renderCount);
	
	textNumSphere->SetText(t);
	textNumSphere->Draw(1.0f, 1.0f, 1.0f, 20.0f, 10.0f);

	t = L"Cam: x = " + intToStr(cam.GetPosition().x) + 
		L" y = " + intToStr(cam.GetPosition().y) + 
		L" z = " + intToStr(cam.GetPosition().z);
	textCamCoord->SetText(t);
	textCamCoord->Draw(1.0f, 1.0f, 1.0f, 20.0f, 30.0f);
	
	TurnZBufferOn();
	TurnOffAlphaBlending();

	return true;
}

void MyRender::Close()
{
	_CLOSE(font);
	_CLOSE(textNumSphere);
	_CLOSE(mesh);
	//_CLOSE(player);
	modelList.Close();
}

Player* MyRender::GetPlayer()
{
	return player;
}

XMFLOAT3 MyRender::GetWorldCords(int x, int y)
{
	XMMATRIX m = XMMatrixInverse(NULL, cam.GetViewMatrix());

	XMFLOAT3 v;
	v.x = (((2.0f * x) / GetWidth()) - 1) / XMVectorGetByIndex( GetProjection().r[0], 0);
	v.y = -(((2.0f * y) / GetHeight()) - 1) / XMVectorGetByIndex(GetProjection().r[1], 1);
	v.z = 1.0f;

	XMVECTOR vPickRayDir = XMVectorSet(
		v.x * XMVectorGetByIndex(m.r[0], 0) + v.y * XMVectorGetByIndex(m.r[1], 0) + v.z * XMVectorGetByIndex(m.r[2], 0),
		v.x * XMVectorGetByIndex(m.r[0], 1) + v.y * XMVectorGetByIndex(m.r[1], 1) + v.z * XMVectorGetByIndex(m.r[2], 1),
		v.x * XMVectorGetByIndex(m.r[0], 2) + v.y * XMVectorGetByIndex(m.r[1], 2) + v.z * XMVectorGetByIndex(m.r[2], 2),
		1
	);

	XMVECTOR vPickRayOrig = XMVectorSet(
		XMVectorGetByIndex(m.r[3], 0),
		XMVectorGetByIndex(m.r[3], 1),
		XMVectorGetByIndex(m.r[3], 2),
		0
	);

	XMVECTOR N = XMVectorSet(0.0f, 1.0f, 0.0f, 0); // Пересечение с плоскостью
	auto t = -XMVector3Dot(vPickRayOrig, N) / XMVector3Dot(vPickRayDir, N);
	auto result = vPickRayOrig + vPickRayDir * t;
	return XMFLOAT3(XMVectorGetByIndex(result, 0), 0, XMVectorGetByIndex(result, 2));
}
