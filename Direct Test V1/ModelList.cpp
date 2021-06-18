#include "ModelList.h"

bool ModelList::Init(int numModels)
{
	modelCount = numModels;
	modelInfoList = new XMFLOAT3[modelCount];
	if (!modelInfoList)
		return false;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < modelCount; i++)
	{
		modelInfoList[i].x = (((float)rand() - (float)rand()) / RAND_MAX) * 20;
		modelInfoList[i].y = (((float)rand() - (float)rand()) / RAND_MAX) * 20;
		modelInfoList[i].z = (((float)rand() - (float)rand()) / RAND_MAX) * 20;
	}
	return true;
}

void ModelList::Close()
{
	_DELETE_ARRAY(modelInfoList);
}

void ModelList::GetData(int index, float& posX, float& posY, float& posZ)
{
	posX = modelInfoList[index].x;
	posY = modelInfoList[index].y;
	posZ = modelInfoList[index].z;
}

