#pragma once

#include "D3D11_Framework.h"

using namespace D3D11Framework;

class ModelList
{
public:
	ModelList()
	{
		modelInfoList = nullptr;
	}

	bool Init(int numModels);
	void Close();

	int GetModelCount()
	{
		return modelCount;
	}

	void GetData(int index, float& posX, float& posY, float& posZ);

private:
	int			modelCount;
	XMFLOAT3*	modelInfoList;
};