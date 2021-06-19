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

void GameObject::InitColider(XMFLOAT3 center, XMFLOAT3 extents)
{
	this->center = center;
	this->extents = extents;
	colider = BoundingBox(XMFLOAT3(pos.x + center.x, pos.y + center.y, pos.z + center.z), extents);
}


