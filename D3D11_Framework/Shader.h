#pragma once

#include <vector>

#include "Render.h"

namespace D3D11Framework
{
//------------------------------------------------------------------

	class Shader
	{
	public:
		Shader(Render *render);
		// ������ ������� �� ������ �������� �� ������ CreateShader
		void AddInputElementDesc(const char *SemanticName, DXGI_FORMAT format);

		bool CreateShader(const wchar_t *namevs,const wchar_t *nameps);
		bool AddTexture(const wchar_t *name);
		void AddTexture(ID3D11ShaderResourceView* texture);

		void Draw();
		void Close();

	private:
		HRESULT m_compileshaderfromfile(const WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);

		Render *m_render;

		ID3D11VertexShader *m_vertexShader;
		ID3D11PixelShader *m_pixelShader;
		ID3D11InputLayout *m_layout;
		std::vector<ID3D11ShaderResourceView *> m_textures;
		
		D3D11_INPUT_ELEMENT_DESC *m_layoutformat;
		unsigned int m_numlayout;
	};

//------------------------------------------------------------------
}