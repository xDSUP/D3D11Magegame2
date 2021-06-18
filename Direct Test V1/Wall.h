#pragma once
#include "D3D11_Framework.h"
#include "GameObject.h"
#include "Model.h"


class Wall : public GameObject
{
public:
	Wall(Model* model)
	{
		this->model = model;
	}
	void Draw(CXMMATRIX viewMatrix) override;

	Model* model;
};

