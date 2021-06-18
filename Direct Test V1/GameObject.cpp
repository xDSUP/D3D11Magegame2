#include "GameObject.h"

void GameObject::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void GameObject::SetRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void GameObject::SetVelosity(float x, float y, float z)
{
	velosity.x = x;
	velosity.y = y;
	velosity.z = z;
}

XMFLOAT3 GameObject::GetVelosity()
{
	return velosity;
}


XMFLOAT3 GameObject::GetPosition()
{
	return pos;
}

XMFLOAT3 GameObject::GetRotation()
{
	return rot;
}

