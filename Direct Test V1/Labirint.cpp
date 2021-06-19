#include "Labirint.h"
#include <fstream>
#include <iostream>

bool Labirint::Init(Model* wallModel, Model* targetModel)
{
    modelOfWall = wallModel;
    modelOfTarget = targetModel;
    return false;
}

bool Labirint::LoadFromFile(const char* path)
{
    std::string line;
    float modelSize = 3;

    std::ifstream in(path); // окрываем файл для чтения
    if (in.is_open())
    {
        Wall* newWall;
        Target* newTarget;
        auto wallWCenter = XMFLOAT3(1.5, 4, 0.75);
        auto wallWExtends = XMFLOAT3(1.5, 4, 0.75);
    	
        auto wallHCenter = XMFLOAT3(0.75, 4, 1.5);
        auto wallHExtends = XMFLOAT3(0.75, 4, 1.5);
    	
        auto targetCenter = XMFLOAT3(1.5, 2, 1.5);
        auto targetExtends = XMFLOAT3(1.5, 2, 1.5);
        int j = 0;
        while (getline(in, line))
        {
            for (size_t i = 0; i < line.size(); i++)
            {
                char ch = line[i];
                switch (ch)
                {
                case 'w':
                    newWall = new Wall(modelOfWall);
                    newWall->SetPosition(i*modelSize, 0, j * modelSize);
                    newWall->InitColider(wallWCenter, wallWExtends);
                    walls.push_back(newWall);
                    break;
                case 'h':
                    newWall = new Wall(modelOfWall);
                    newWall->SetRotation(0, 90, 0);
                    newWall->SetPosition(i * modelSize, 0, j * modelSize);
                    newWall->InitColider(wallHCenter, wallHExtends);
                    walls.push_back(newWall);
                    break;
                case 'm':
                    newTarget = new Target(modelOfTarget);
                    newTarget->SetRotation(0, 45, 0);
                    newTarget->SetPosition(i * modelSize, 0, j * modelSize);
                    newTarget->InitColider(targetCenter, targetExtends);
                    targets.push_back(newTarget);
                    break;
                case 's':
                    this->spawnPlayer = XMFLOAT3(i * modelSize, 0, j * modelSize);
                    break;
                case 'e':
                    newWall = new Wall(modelOfWall);
                    newWall->SetRotation(0, 90, 0 );
                    newWall->SetPosition(i * modelSize, 0, j * modelSize);
                    newWall->InitColider(wallWCenter, wallWExtends);
                    walls.push_back(newWall);
                    exit = newWall;
                    break;
                default:
                    break;
                }
            }
            j++;
        }
    }
    else
        return false;
    in.close();     // закрываем файл
	return false;
}

void Labirint::Draw(CXMMATRIX viewMatrix)
{
    for (size_t i = 0; i < walls.size(); i++)
    {
        walls[i]->Draw(viewMatrix);
    }

    for (size_t i = 0; i < targets.size(); i++)
    {
        targets[i]->Draw(viewMatrix);
    }
}

