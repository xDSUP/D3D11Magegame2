#pragma once
#include "D3D11_Framework.h"
#include "Drawable.h"
#include <DirectXCollision.h>

using namespace D3D11Framework;

class GameObject : public Drawable
{
public :
	GameObject() = default;
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetVelosity(float x, float y, float z);
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	void InitColider(XMFLOAT3 center, XMFLOAT3 extents);
	XMFLOAT3 GetVelosity();
	void virtual Draw(CXMMATRIX view) = 0;

	BoundingBox colider;

protected:
	XMFLOAT3 center;
	XMFLOAT3 extents;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 velosity;
};

