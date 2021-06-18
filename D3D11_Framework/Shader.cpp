#include "stdafx.h"
#include "Shader.h"
#include "macros.h"
#include "Log.h"

using namespace D3D11Framework;

#define MAXLAYOUT 8

Shader::Shader(Render *render)
{
	m_render = render;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_layoutformat = nullptr;
	m_numlayout = 0;
}

void Shader::AddInputElementDesc(const char *SemanticName, DXGI_FORMAT format)
{
	if (!m_numlayout)
	{
		m_layoutformat = new D3D11_INPUT_ELEMENT_DESC[MAXLAYOUT];
		if (!m_layoutformat)
			return;
	}
	else if (m_numlayout >= MAXLAYOUT)
		return;

	D3D11_INPUT_ELEMENT_DESC &Layout = m_layoutformat[m_numlayout];

	Layout.SemanticName = SemanticName;
	Layout.SemanticIndex = 0;
	Layout.Format = format;
	Layout.InputSlot = 0;
	if ( !m_numlayout )
		Layout.AlignedByteOffset = 0;
	else
		Layout.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	Layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	Layout.InstanceDataStepRate = 0;

	m_numlayout++;
}

bool Shader::CreateShader(const wchar_t *namevs, const wchar_t *nameps)
{
	HRESULT hr = S_OK;
	ID3DBlob *vertexShaderBuffer = nullptr;
	hr = m_compileshaderfromfile(namevs, "main", "vs_4_0", &vertexShaderBuffer);
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ��������� ��������� ������ %ls", namevs);
		return false;
	}

	ID3DBlob *pixelShaderBuffer = nullptr;
	hr = m_compileshaderfromfile(nameps, "main", "ps_4_0", &pixelShaderBuffer);
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ��������� ���������� ������ %ls", nameps);
		return false;
	}

	hr = m_render->m_pd3dDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ������� ��������� ������");
		return false;
	}

	hr = m_render->m_pd3dDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ������� ���������� ������");
		return false;
	}

	hr = m_render->m_pd3dDevice->CreateInputLayout(m_layoutformat, m_numlayout, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ������� ������ �����");
		return false;
	}
	_DELETE_ARRAY(m_layoutformat);

	_RELEASE(vertexShaderBuffer);
	_RELEASE(pixelShaderBuffer);

	return true;
}

HRESULT Shader::m_compileshaderfromfile(const WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut )
{
	HRESULT hr = S_OK;

	DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob *pErrorBlob = nullptr;
	hr = D3DX11CompileFromFile(FileName, NULL, NULL, EntryPoint, ShaderModel, ShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL );
	if( FAILED(hr) && pErrorBlob )
		Log::Get()->Err( (char*)pErrorBlob->GetBufferPointer() );

	_RELEASE(pErrorBlob);
	return hr;
}

bool Shader::AddTexture(const wchar_t *name)
{
	ID3D11ShaderResourceView *texture = nullptr;
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile( m_render->m_pd3dDevice, name, NULL, NULL, &texture, NULL );
	if( FAILED(hr) )
	{
		Log::Get()->Err("�� ������� ��������� �������� %ls", name);
		return false;
	}

	AddTexture(texture);

	return true;
}

void Shader::AddTexture(ID3D11ShaderResourceView* texture)
{
	m_textures.push_back(texture);
}

void Shader::Draw()
{
	m_render->m_pImmediateContext->IASetInputLayout(m_layout);
	m_render->m_pImmediateContext->VSSetShader(m_vertexShader, NULL, 0);
	m_render->m_pImmediateContext->PSSetShader(m_pixelShader, NULL, 0);
	if (!m_textures.empty())
		m_render->m_pImmediateContext->PSSetShaderResources(0, m_textures.size(), &m_textures[0]);
	else
		m_render->m_pImmediateContext->PSSetShaderResources(0, 0, nullptr);
}

void Shader::Close()
{
	_RELEASE(m_vertexShader);
	_RELEASE(m_pixelShader);
	_RELEASE(m_layout);
	for (int i = 0; i < m_textures.size(); i++)
		_RELEASE(m_textures[i]);
	m_textures.clear();
}