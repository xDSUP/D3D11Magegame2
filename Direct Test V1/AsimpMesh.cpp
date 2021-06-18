#include "AsimpMesh.h"

#include "Util.h"


void AsimpMesh::Init()
{
	StaticMesh::Init(w("123"));
}

bool AsimpMesh::m_loadFromFile(wchar_t* name)
{
	// эта штука и недолжна грузится из файла
	//throw 0;
	return true;
}
