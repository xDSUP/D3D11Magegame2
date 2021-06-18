#pragma once
#include "D3D11_Framework.h"

using namespace D3D11Framework;
using namespace std;


class AsimpMesh : public StaticMesh
{
public:
	explicit AsimpMesh(Render* render)
		: StaticMesh(render)
	{
	}

	void Init();
protected:
	bool m_loadFromFile(wchar_t* name) override;

private:
	
};

