#pragma once

#include "Render.h"
#include "LightHelper.h"

namespace D3D11Framework
{
	//------------------------------------------------------------------
	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};


	struct VSConstantBuffer
	{
		XMMATRIX WVP;
		XMMATRIX world;
		XMMATRIX worldInvTranspose;
	};

	struct PSConstantBuffer
	{
		DirectionalLight dirLight [4] ;
		PointLight pointLight[10];
		SpotLight spotLight[4];
		Material material;
		XMFLOAT3 eyePos;
		float numDirLight;
		float numPointLight;
		float numSpotLight;
		float pad1;
		float pad2;
	};
	
		
	class StaticMesh
	{
	public:
		StaticMesh(Render *render);

		bool Init(wchar_t *name);
		void Draw(CXMMATRIX viewmatrix);
		void Close();

		void Identity();
		void Translate(float x, float y, float z);
		void Rotate(float angle, float x, float y, float z);
		void Scale(float x, float y, float z);
		

		bool InitBuffers(unsigned short VertexCount, unsigned short indexCount, unsigned short* indices, Vertex* vertices);

		Shader* GetShader()
		{
			return m_shader;
		}
		

		void* operator new(size_t i)
		{
			return _aligned_malloc(i,16);
		}

		void operator delete(void* p)
		{
			_aligned_free(p);
		}

	protected:
		virtual bool m_loadFromFile(wchar_t* name) = 0;

		void m_RenderBuffers();
		void m_SetShaderParameters(CXMMATRIX viewmatrix);
		void m_RenderShader();

		Render *m_render;

		ID3D11Buffer *m_vertexBuffer;
		ID3D11Buffer *m_indexBuffer;
		ID3D11Buffer *m_VSConstantBuffer;
		ID3D11Buffer* m_PSConstantBuffer;
		PSConstantBuffer psConstantBuffer;
		VSConstantBuffer vsConstantBuffer;
		Shader *m_shader;

		Material m_material;

		XMMATRIX m_objMatrix;
		unsigned short m_indexCount;
	};

	//------------------------------------------------------------------
}