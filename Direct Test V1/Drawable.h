#pragma once
#include "D3D11_Framework.h"

class Drawable
{
public:
	virtual void Draw(CXMMATRIX viewMatrix) = 0;
};