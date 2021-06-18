#include "stdafx.h"
#include "MeshMS3D.h"
#include "stdafx.h"
#include "StaticMesh.h"
#include "ms3dspec.h"
#include <fstream>
#include "macros.h"
#include "Util.h"
#include "Shader.h"
#include "Buffer.h"

using namespace D3D11Framework;



bool D3D11Framework::MeshMS3D::m_loadFromFile(wchar_t* filename)
{
	unsigned short VertexCount = 0;
	unsigned short TriangleCount = 0;
	unsigned short GroupCount = 0;
	unsigned short MaterialCount = 0;
	MS3DVertex* pMS3DVertices = nullptr;
	MS3DTriangle* pMS3DTriangles = nullptr;
	MS3DGroup* pMS3DGroups = nullptr;
	MS3DMaterial* pMS3DMaterials = nullptr;
	MS3DHeader header;

	ifstream fin;

	fin.open(filename, std::ios::binary);
	fin.read((char*)(&(header)), sizeof(header));
	if (header.version != 3 && header.version != 4)
		return false;

	fin.read((char*)(&VertexCount), sizeof(unsigned short));
	pMS3DVertices = new MS3DVertex[VertexCount];
	fin.read((char*)pMS3DVertices, VertexCount * sizeof(MS3DVertex));

	fin.read((char*)(&TriangleCount), sizeof(unsigned short));
	pMS3DTriangles = new MS3DTriangle[TriangleCount];
	fin.read((char*)pMS3DTriangles, TriangleCount * sizeof(MS3DTriangle));

	fin.read((char*)(&GroupCount), sizeof(unsigned short));
	pMS3DGroups = new MS3DGroup[GroupCount];
	for (int i = 0; i < GroupCount; i++)
	{
		fin.read((char*)&(pMS3DGroups[i].flags), sizeof(unsigned char));
		fin.read((char*)&(pMS3DGroups[i].name), sizeof(char[32]));
		fin.read((char*)&(pMS3DGroups[i].numtriangles), sizeof(unsigned short));
		unsigned short triCount = pMS3DGroups[i].numtriangles;
		pMS3DGroups[i].triangleIndices = new unsigned short[triCount];
		fin.read((char*)(pMS3DGroups[i].triangleIndices), sizeof(unsigned short) * triCount);
		fin.read((char*)&(pMS3DGroups[i].materialIndex), sizeof(char));
	}

	fin.read((char*)(&MaterialCount), sizeof(unsigned short));
	pMS3DMaterials = new MS3DMaterial[MaterialCount];
	fin.read((char*)pMS3DMaterials, MaterialCount * sizeof(MS3DMaterial));

	fin.close();

	m_indexCount = TriangleCount * 3;
	unsigned short* indices = new unsigned short[m_indexCount];
	if (!indices)
		return false;
	Vertex* vertices = new Vertex[VertexCount];
	if (!vertices)
		return false;

	for (int i = 0; i < TriangleCount; i++)
	{
		indices[3 * i + 0] = pMS3DTriangles[i].vertexIndices[0];
		indices[3 * i + 1] = pMS3DTriangles[i].vertexIndices[1];
		indices[3 * i + 2] = pMS3DTriangles[i].vertexIndices[2];
	}

	for (int i = 0; i < VertexCount; i++)
	{
		vertices[i].Pos.x = pMS3DVertices[i].vertex[0];
		vertices[i].Pos.y = pMS3DVertices[i].vertex[1];
		vertices[i].Pos.z = pMS3DVertices[i].vertex[2];

		for (int j = 0; j < TriangleCount; j++)
		{
			if (i == pMS3DTriangles[j].vertexIndices[0])
			{
				vertices[i].Tex.x = pMS3DTriangles[j].s[0];
				vertices[i].Tex.y = pMS3DTriangles[j].t[0];
			}
			else if (i == pMS3DTriangles[j].vertexIndices[1])
			{
				vertices[i].Tex.x = pMS3DTriangles[j].s[1];
				vertices[i].Tex.y = pMS3DTriangles[j].t[1];
			}
			else if (i == pMS3DTriangles[j].vertexIndices[2])
			{
				vertices[i].Tex.x = pMS3DTriangles[j].s[2];
				vertices[i].Tex.y = pMS3DTriangles[j].t[2];
			}
			else
				continue;
			break;
		}
	}

	wchar_t* name = CharToWChar(pMS3DMaterials[0].texture);
	if (!m_shader->AddTexture(name))
		return false;
	_DELETE_ARRAY(name);

	_DELETE_ARRAY(pMS3DMaterials);
	if (pMS3DGroups != nullptr)
	{
		for (int i = 0; i < GroupCount; i++)
			_DELETE_ARRAY(pMS3DGroups[i].triangleIndices);
		_DELETE_ARRAY(pMS3DGroups);
	}
	_DELETE_ARRAY(pMS3DTriangles);
	_DELETE_ARRAY(pMS3DVertices);

	bool value1;
	if (!InitBuffers(VertexCount, m_indexCount, indices, vertices))
		return false;

	_DELETE_ARRAY(vertices);
	_DELETE_ARRAY(indices);

	return true;
}
