#pragma once
#include "GameObject.h"
#include "Model.h"

class Target :
    public GameObject
{
public:
	Target(Model* model)
	{
		this->model = model;
	}
	void Draw(CXMMATRIX viewMatrix) override;

	Model* model;
};

