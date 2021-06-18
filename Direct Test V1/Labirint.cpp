#include "Labirint.h"
#include <fstream>
#include <iostream>

bool Labirint::Init(Model* model)
{
    modelOfWall = model;
    return false;
}

bool Labirint::LoadFromFile(const char* path)
{
    std::string line;

    std::ifstream in(path); // окрываем файл для чтения
    if (in.is_open())
    {
        Wall* newWall;
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
                    newWall->SetPosition(i, 0, j);
                    walls.push_back(newWall);
                    break;
                case 'h':
                    newWall = new Wall(modelOfWall);
                    newWall->SetRotation(0, 90, 0);
                    newWall->SetPosition(i, 0, j);
                    walls.push_back(newWall);
                    break;
                case ' ':
                    break;
                case 's':
                    this->spawnPlayer = XMFLOAT3(i, 0, j);
                    break;
                case 'e':
                    newWall = new Wall(modelOfWall);
                    newWall->SetRotation(0, 90, 0);
                    newWall->SetPosition(i, 0, j);
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
}
