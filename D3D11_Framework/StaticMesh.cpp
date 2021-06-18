#include "stdafx.h"
#include "StaticMesh.h"
#include "ms3dspec.h"
#include <fstream>
#include "macros.h"
#include "Util.h"
#include "Shader.h"
#include "Buffer.h"
#include "Log.h"
#include "comutil.h"
#include <comdef.h>

using namespace D3D11Framework;
using namespace std;



bool StaticMesh::InitBuffers(unsigned short VertexCount, unsigned short indexCount, unsigned short* indices, Vertex* vertices)
{
	m_vertexBuffer = Buffer::CreateVertexBuffer(m_render->GetDevice(), sizeof(Vertex) * VertexCount, false, vertices);
	if (!m_vertexBuffer)
		return false;

	m_indexCount = indexCount;
	m_indexBuffer = Buffer::CreateIndexBuffer(m_render->GetDevice(), sizeof(unsigned short) * indexCount, false, indices);
	if (!m_indexBuffer)
		return false;


	m_VSConstantBuffer = Buffer::CreateConstantBuffer(m_render->GetDevice(), sizeof(VSConstantBuffer), false);
	if (!m_VSConstantBuffer)
		return false;

	m_PSConstantBuffer = Buffer::CreateConstantBuffer(m_render->GetDevice(), sizeof(PSConstantBuffer), true);
	if (!m_PSConstantBuffer)
	{
		return false;
	}
	return true;
}

StaticMesh::StaticMesh(Render *render)
: m_PSConstantBuffer(nullptr), m_indexCount(0)
{
	m_render = render;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_VSConstantBuffer = nullptr;
	m_shader = nullptr;
}

bool StaticMesh::Init(wchar_t *name)
{
	Identity();

	m_shader = new Shader(m_render);
	if ( !m_shader )
		return false;

	m_shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	m_shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);
	
	//m_shader->AddInputElementDesc("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
	if ( !m_shader->CreateShader(L"meshVS.hlsl", L"meshPS.hlsl") )
		return false;

	if( !m_loadFromFile(name) )
		return false;

	return true;
}


void StaticMesh::Draw(CXMMATRIX viewmatrix)
{
	m_RenderBuffers();
	m_SetShaderParameters(viewmatrix);
	m_RenderShader();
}

void StaticMesh::m_RenderBuffers()
{
	unsigned int stride = sizeof(Vertex); 
	unsigned int offset = 0;
	m_render->m_pImmediateContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	m_render->m_pImmediateContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	/*m_render->m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);*/
}

void StaticMesh::m_SetShaderParameters(CXMMATRIX viewmatrix)
{
	XMMATRIX WVP = m_objMatrix * viewmatrix * m_render->m_Projection;

	
	vsConstantBuffer.world = XMMatrixTranspose(m_objMatrix);
	vsConstantBuffer.WVP = XMMatrixTranspose(WVP);
	vsConstantBuffer.worldInvTranspose = XMMatrixTranspose(InverseTranspose(m_objMatrix));
	m_render->m_pImmediateContext->UpdateSubresource( m_VSConstantBuffer, 0, NULL, &vsConstantBuffer, 0, 0 );
	
	for (size_t i = 0; i < m_render->NumDirLight(); i++)
		psConstantBuffer.dirLight[i] = m_render->GetDirectionalLights()[i];

	int i = 0;
	for (auto l : m_render->GetPointLights())
	{
		memcpy_s(&psConstantBuffer.pointLight[i], sizeof(PointLight), &l->light, sizeof(PointLight));
		i++;
	}
	
	for (size_t i = 0; i < m_render->NumSpotLight(); i++)
		psConstantBuffer.spotLight[i] = m_render->GetSpotLights()[i];
	
	psConstantBuffer.numDirLight = m_render->NumDirLight();
	psConstantBuffer.numPointLight = m_render->NumPointLight();
	psConstantBuffer.numSpotLight = m_render->NumSpotLight();

	if(m_material.diffuse.x > 0)
	{
		psConstantBuffer.material.ambient = m_material.ambient;
		psConstantBuffer.material.diffuse =	m_material.diffuse;
		psConstantBuffer.material.specular = m_material.specular;
		psConstantBuffer.material.reflect = m_material.reflect;
	}
	else
	{
		//cbPS.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		psConstantBuffer.material.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		psConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		psConstantBuffer.material.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 5.0f);
	}
	
	psConstantBuffer.eyePos = m_render->GetCam()->GetPosition();

	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_render->m_pImmediateContext->Map(m_PSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &psConstantBuffer, sizeof(PSConstantBuffer));
	m_render->m_pImmediateContext->Unmap(m_PSConstantBuffer, 0);


	m_render->m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_VSConstantBuffer);
	m_render->m_pImmediateContext->PSSetConstantBuffers(1, 1, &m_PSConstantBuffer);
}

void StaticMesh::m_RenderShader()
{
	m_shader->Draw();
	m_render->m_pImmediateContext->DrawIndexed(m_indexCount, 0, 0);
}

void StaticMesh::Close()
{
	_RELEASE(m_indexBuffer);
	_RELEASE(m_vertexBuffer);
	_RELEASE(m_VSConstantBuffer);
	_RELEASE(m_PSConstantBuffer);
	_CLOSE(m_shader);
}

void StaticMesh::Translate(float x, float y, float z)
{
	m_objMatrix *= XMMatrixTranslation(x,y,z);
}

void StaticMesh::Rotate(float angle, float x, float y, float z)
{
	XMVECTOR v = XMVectorSet(x,y,z,0.0f);
	m_objMatrix *= XMMatrixRotationAxis(v,angle);
}

void StaticMesh::Scale(float x, float y, float z)
{
	m_objMatrix *= XMMatrixScaling(x,y,z);
}

void StaticMesh::Identity()
{
	m_objMatrix = XMMatrixIdentity();
}