#include "Target.h"

#include "Util.h"

void Target::Draw(CXMMATRIX viewMatrix)
{
	model->Identity();
	model->Rotate(convertDegreeToRad(rot.y), 0, 1, 0);
	model->Translate(pos.x, pos.y, pos.z);

	model->Draw(viewMatrix);
	return;
}
