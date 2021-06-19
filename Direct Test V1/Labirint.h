#pragma once

#include "D3D11_Framework.h"
#include "Wall.h"
#include "GameObject.h"
#include "Model.h"
#include "Target.h"

using namespace D3D11Framework;
using namespace std;

class Labirint : public GameObject
{
public :
	bool Init(Model* wallModel, Model* targetModel);

	/// <summary>
	/// Загружает лабиринт из файла
	/// </summary>
	/// <param name="path"></param>
	/// <returns></returns>
	bool LoadFromFile(const char* path); 
	void Draw(CXMMATRIX viewMatrix) override;

	vector<Wall*> walls;
	vector<Target*> targets;
	Wall* exit;
	XMFLOAT3 spawnPlayer;
	Model* modelOfWall;
	Model* modelOfTarget;
};

