#pragma once
#include "Model.h"
#include "MovableGameObject.h"
#include "Util.h"
#include "ParticleGenerator.h"

class FireBall :
    public MovableGameObject
{
public:
	void InitPointLight()
	{
		light.position = XMFLOAT3(pos.x, pos.y, pos.z);
		light.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		light.diffuse = XMFLOAT4(1.0f, 0.55f, 0.0f, 1.0f);
		light.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light.att = XMFLOAT3(0.0f, 0.1f, 0.2f);
		//m_PointLight.color = XMFLOAT3(0.5f, 0.1f, 0.0f);
		light.range = 5.0f;
	}

	void InitParticles(ParticleGenerator* gen)
	{
		particles = gen;
	}

	void Draw(CXMMATRIX view) override
	{
		model->Draw(view);
		
		particles->Draw(view);
	}

	void UpdatePos(float frameTime)
	{
		lifeTime -= frameTime;
		this->frameTime = frameTime;
		MoveForward(true);
		model->Identity();
		model->Scale(0.5f, 0.5f, 0.5f);
		float rad = convertDegreeToRad(rot.y);
		velosity.x = (forwardMoveSpeed)*sinf(rad);
		velosity.z = (forwardMoveSpeed)*cosf(rad);
		pos.x += velosity.x;
		pos.z += velosity.z;

		light.position = pos;
		model->Translate(pos.x, pos.y, pos.z);
		particles->Update(frameTime, pos, 30);

	}

	PointLight	light;
	StaticMesh* model;
	float lifeTime;

	ParticleGenerator* particles;
private:
	

};

