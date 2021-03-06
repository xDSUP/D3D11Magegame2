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
	wallModel->Init("wallBig.obj");

	targetModel = new Model(this);
	targetModel->Init("target.obj");

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(m_pd3dDevice, w("fireBallParticle.jpg"), NULL, NULL, &fireBallParticleTexture, NULL);
	if (FAILED(hr))
	{
		Log::Get()->Err("Не удалось загрузить текстуру %ls", "fireBallParticle.png");
		return false;
	}
	
	labirint->Init(wallModel, targetModel);
	if (labirint->LoadFromFile("1.txt"))
		return false;
	//labirint = new Model(this);
	//labirint->Init(("maze.obj"));

	player = new Player();
	player->InitModel(this, "grim.obj");
	player->SetSpeedTurn(2);
	player->SetSpeedMove(4);
	player->SetMaxFrameTime(3);
	player->SetPosition(labirint->spawnPlayer.x, labirint->spawnPlayer.y, labirint->spawnPlayer.z);
	player->InitColider(XMFLOAT3(1, 4, 0.5), XMFLOAT3(1, 4, 0.5));
	
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
	
	if (!flourModel->Init("bigFlout.obj"))
		return false;

	flourModel->Scale(-1, 1, -1);

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
				bool intersect = false;
				for (auto wall : labirint->walls)
				{
					if(wall->colider.Intersects(fireBall->colider))
					{
						sLog->Debug("Фаер(%.3lf, %.3lf, %.3lf) пересекся со стеной(%.3lf, %.3lf, %.3lf)",
							fireBall->GetPosition().x, fireBall->GetPosition().y, fireBall->GetPosition().z,
							wall->GetPosition().x, wall->GetPosition().y, wall->GetPosition().z);
						intersect = true;
						break;
					}
				}
				for (auto target : labirint->targets)
				{
					if (target->colider.Intersects(fireBall->colider))
					{
						sLog->Debug("Фаер(%.3lf, %.3lf, %.3lf) пересекся с врагом(%.3lf, %.3lf, %.3lf)",
							fireBall->GetPosition().x, fireBall->GetPosition().y, fireBall->GetPosition().z,
							target->GetPosition().x, target->GetPosition().y, target->GetPosition().z);
						intersect = true;
						break;
					}

				}
				if(intersect)
				{
					delFireballs.push_back(fireBall);
					it = m_PointLights.erase(it);
				}
				else
				{
					std::advance(it, 1);
				}
			}
			i++;
		}
		//auto iter = delIndFireballs.begin();
		for (FireBall* fireBall : delFireballs)
		{
			fireBalls.remove(fireBall);
			//std::advance(it, iter._Ptr->_Myval); // <-- advance ��������� ���������� �������� �� k �������
			delete fireBall->particles;
			// <--- ������ �������� �� k+1 �������, ����� it ��� *
			delete fireBall;
		}
	}
}

int MyRender::drawLabirint(XMMATRIX viewMatrix)
{
	int rendered = 0;
	for (int i = 0; i < labirint->walls.size(); i++)
	{
		auto pos = labirint->walls[i]->GetPosition();
		bool renderModel = frustum.CheckRectangle(pos.x, pos.y, pos.z, 3, 1, 10);
		if(renderModel)
		{
			rendered++;
			labirint->walls[i]->Draw(viewMatrix);
		}
		
	}
	for (size_t i = 0; i < labirint->targets.size(); i++)
	{
		labirint->targets[i]->Draw(viewMatrix);
	}
	return rendered;
}

void MyRender::drawPlayer(XMMATRIX viewMatrix)
{
	bool intersect = false;
	if(player->IsMoved())
	{
		for (auto wall : labirint->walls)
		{
			if (wall->colider.Intersects(player->colider))
			{
				sLog->Debug("Фраер(%.3lf, %.3lf, %.3lf) пересекся со стеной(%.3lf, %.3lf, %.3lf)",
					player->GetPosition().x, player->GetPosition().y, player->GetPosition().z,
					wall->GetPosition().x, wall->GetPosition().y, wall->GetPosition().z);
				intersect = true;
				break;
			}
		}
		if(intersect)
		{
			player->SetMoveBack(true);
			player->SetMoveForward(false);
		}
	}
	player->Draw(viewMatrix);
	if (intersect)
		player->SetMoveBack(false);

	mesh->Identity();
	mesh->Scale(0.5, 0.5, 0.5);
	mesh->Translate(
		player->GetTorchLight()->position.x, 
		player->GetTorchLight()->position.y, 
		player->GetTorchLight()->position.z
	);
	mesh->Draw(viewMatrix);
}

bool MyRender::Draw()
{
	timer.Frame();
	handleCamMove();
	auto frameTime = timer.GetFrameTime();
	cam.Render(frameTime);
	

	XMMATRIX viewMatrix = cam.GetViewMatrix();
	frustum.ConstructFrustum(1000, m_Projection, viewMatrix);

	int renderCount = drawLabirint(viewMatrix);;
	
	flourModel->Draw(viewMatrix);
	
	drawPlayer(viewMatrix);
	
	TurnOnAlphaBlending();

	m_renderstate->TurnRasterNoCull();
	updateAndDrawFireBalls(viewMatrix);

	torchParticleGenerator->Update(frameTime, player->GetTorchLight()->position, 2);
	torchParticleGenerator->Draw(viewMatrix);

	m_renderstate->TurnRasterNormal();
	
	TurnZBufferOff();
	std::wstring t = L"Стеночек: " + intToStr(renderCount);
	
	textNumSphere->SetText(t);
	textNumSphere->Draw(1.0f, 1.0f, 1.0f, 20.0f, 10.0f);

	t = L"Врагов: " + intToStr(labirint->targets.size());
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
		XMVectorGetByIndex(m.r[3], 1)-2, // смещение камеры вниз, чтобы поднять плоскость пола
		XMVectorGetByIndex(m.r[3], 2),
		0
	);

	XMVECTOR N = XMVectorSet(0.0f, 1.0f, 0.0f, 0); // ����������� � ����������
	auto t = -XMVector3Dot(vPickRayOrig, N) / XMVector3Dot(vPickRayDir, N);
	auto result = vPickRayOrig + vPickRayDir * t;
	return XMFLOAT3(XMVectorGetByIndex(result, 0), 0, XMVectorGetByIndex(result, 2));
}
