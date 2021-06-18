#include "stdafx.h"
#include "Frustum.h"

using namespace D3D11Framework;

void Frustum::ConstructFrustum(float screenDepth, CXMMATRIX projectionMatrix, CXMMATRIX viewMatrix)
{
	XMMATRIX projMatrix = projectionMatrix;
	// Вычисление минимальной дистации по Z в фрустуме.
	float zMinimum = -(float)(XMVectorGetByIndex(projMatrix.r[3], 2) / XMVectorGetByIndex(projMatrix.r[2], 2));
	float r = screenDepth / (screenDepth - zMinimum);
	XMVectorSetByIndex(projMatrix.r[2], r, 2);
	XMVectorSetByIndex(projMatrix.r[3], r * zMinimum, 2); // _43

	// Создание матрицы фрустума из видовой и обновленой проекционной матриц.
	XMMATRIX matrix = XMMatrixMultiply(viewMatrix, projMatrix);

	float a, b, c, d;

	// Вычисление близкой (near) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) + XMVectorGetByIndex(matrix.r[0], 2);
	b = XMVectorGetByIndex(matrix.r[1], 3) + XMVectorGetByIndex(matrix.r[1], 2);
	c = XMVectorGetByIndex(matrix.r[2], 3) + XMVectorGetByIndex(matrix.r[2], 2);
	d = XMVectorGetByIndex(matrix.r[3], 3) + XMVectorGetByIndex(matrix.r[3], 2);
	m_planes[0] = XMVectorSet(a, b, c, d);
	m_planes[0] = XMPlaneNormalize(m_planes[0]);

	// Вычисление дальней (far) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) - XMVectorGetByIndex(matrix.r[0], 2);
	b = XMVectorGetByIndex(matrix.r[1], 3) - XMVectorGetByIndex(matrix.r[1], 2);
	c = XMVectorGetByIndex(matrix.r[2], 3) - XMVectorGetByIndex(matrix.r[2], 2);
	d = XMVectorGetByIndex(matrix.r[3], 3) - XMVectorGetByIndex(matrix.r[3], 2);
	m_planes[1] = XMVectorSet(a, b, c, d);
	m_planes[1] = XMPlaneNormalize(m_planes[1]);

	// Вычисление левой (left) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) + XMVectorGetByIndex(matrix.r[0], 0);
	b = XMVectorGetByIndex(matrix.r[1], 3) + XMVectorGetByIndex(matrix.r[1], 0);
	c = XMVectorGetByIndex(matrix.r[2], 3) + XMVectorGetByIndex(matrix.r[2], 0);
	d = XMVectorGetByIndex(matrix.r[3], 3) + XMVectorGetByIndex(matrix.r[3], 0);
	m_planes[2] = XMVectorSet(a, b, c, d);
	m_planes[2] = XMPlaneNormalize(m_planes[2]);

	// Вычисление правой (right) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) - XMVectorGetByIndex(matrix.r[0], 0);
	b = XMVectorGetByIndex(matrix.r[1], 3) - XMVectorGetByIndex(matrix.r[1], 0);
	c = XMVectorGetByIndex(matrix.r[2], 3) - XMVectorGetByIndex(matrix.r[2], 0);
	d = XMVectorGetByIndex(matrix.r[3], 3) - XMVectorGetByIndex(matrix.r[3], 0);
	m_planes[3] = XMVectorSet(a, b, c, d);
	m_planes[3] = XMPlaneNormalize(m_planes[3]);

	// Вычисление верхней (top) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) - XMVectorGetByIndex(matrix.r[0], 1);
	b = XMVectorGetByIndex(matrix.r[1], 3) - XMVectorGetByIndex(matrix.r[1], 1);
	c = XMVectorGetByIndex(matrix.r[2], 3) - XMVectorGetByIndex(matrix.r[2], 1);
	d = XMVectorGetByIndex(matrix.r[3], 3) - XMVectorGetByIndex(matrix.r[3], 1);
	m_planes[4] = XMVectorSet(a, b, c, d);
	m_planes[4] = XMPlaneNormalize(m_planes[4]);

	// Вычисление нижней (bottom) плоскости.
	a = XMVectorGetByIndex(matrix.r[0], 3) + XMVectorGetByIndex(matrix.r[0], 1);
	b = XMVectorGetByIndex(matrix.r[1], 3) + XMVectorGetByIndex(matrix.r[1], 1);
	c = XMVectorGetByIndex(matrix.r[2], 3) + XMVectorGetByIndex(matrix.r[2], 1);
	d = XMVectorGetByIndex(matrix.r[3], 3) + XMVectorGetByIndex(matrix.r[3], 1);
	m_planes[5] = XMVectorSet(a, b, c, d);
	m_planes[5] = XMPlaneNormalize(m_planes[5]);
}

bool Frustum::CheckPoint(float x, float y, float z)
{
	for(int i=0; i<6; i++) 
	{
		float ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet(x,y,z,1.0f)) );
		if(ret < 0.0f)
			return false;
	}

	return true;
}


bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float size)
{
	for(int i=0; i<6; i++) 
	{
		float ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - size), (yCenter - size), (zCenter - size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + size), (yCenter - size), (zCenter - size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - size), (yCenter + size), (zCenter - size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + size), (yCenter + size), (zCenter - size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - size), (yCenter - size), (zCenter + size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + size), (yCenter - size), (zCenter + size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - size), (yCenter + size), (zCenter + size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + size), (yCenter + size), (zCenter + size), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		return false;
	}

	return true;
}

bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	for(int i=0; i<6; i++) 
	{
		float ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet(xCenter,yCenter,zCenter,1.0f)) );
		if(ret < -radius)
			return false;
	}

	return true;
}

bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	for(int i=0; i<6; i++)
	{
		float ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		ret = XMVectorGetX (XMPlaneDotCoord(m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 1.0f)) );
		if(ret >= 0.0f)
			continue;

		return false;
	}

	return true;
}