#include "stdafx.h"
#include "Buffer.h"

#include "Log.h"
#include "comutil.h"
#include <comdef.h>
#include <iostream>

using namespace D3D11Framework;

ID3D11Buffer* Buffer::CreateVertexBuffer(ID3D11Device *device, int size, bool dynamic, const void *Mem)
{
	ID3D11Buffer *vb = nullptr;

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = size;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	if (dynamic)
	{
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.CPUAccessFlags = 0;
	}	

	D3D11_SUBRESOURCE_DATA Data;	
	Data.pSysMem = Mem;
	Data.SysMemPitch = 0;
	Data.SysMemSlicePitch = 0;
	HRNull(device->CreateBuffer(&BufferDesc, &Data, &vb));

	return vb;
}

ID3D11Buffer* Buffer::CreateIndexBuffer(ID3D11Device *device, int size, bool dynamic, const void *Mem)
{
	ID3D11Buffer *ib = nullptr;

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.ByteWidth = size;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;	
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;
	if (dynamic)
	{
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.CPUAccessFlags = 0;
	}	

	D3D11_SUBRESOURCE_DATA Data;	
	Data.pSysMem = Mem;
	Data.SysMemPitch = 0;
	Data.SysMemSlicePitch = 0;
	HRNull(device->CreateBuffer(&BufferDesc, &Data, &ib));
	return ib;
}

ID3D11Buffer* Buffer::CreateConstantBuffer(ID3D11Device *device, int size, bool dynamic)
{
	ID3D11Buffer *cb = nullptr;

	D3D11_BUFFER_DESC BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(BufferDesc));
	BufferDesc.ByteWidth = size;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	if (dynamic)
	{
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.CPUAccessFlags = 0;
	}	

	HRNull(device->CreateBuffer(&BufferDesc, nullptr, &cb));
	return cb;
}