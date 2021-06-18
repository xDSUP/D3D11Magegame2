#include "Wall.h"
#include "Util.h"


void Wall::Draw(CXMMATRIX viewMatrix)
{
	model->Identity();
	model->Rotate(convertDegreeToRad(rot.y), 0, 1, 0);
	model->Translate(pos.x, pos.y, pos.z);
	model->Draw(viewMatrix);
	return;
}
