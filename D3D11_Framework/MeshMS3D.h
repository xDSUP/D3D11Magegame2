#pragma once
#include "StaticMesh.h"

namespace D3D11Framework
{
	class MeshMS3D :public  StaticMesh
	{
	public:
		MeshMS3D(Render* render) :StaticMesh(render)
		{
			
		}
	protected:
		bool m_loadFromFile(wchar_t* name) override;
	};
}


