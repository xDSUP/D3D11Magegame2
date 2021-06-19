#include "ParticleGenerator.h"

#include "Util.h"



bool ParticleGenerator::Init(wchar_t* name, int amount, XMFLOAT3 velosity, float size, float life)
{
	this->velosity = velosity;
	this->amount = amount;
	this->size = size;
	this->life = life;
	
	shader = new Shader(render);
	shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	if (!shader->CreateShader(w("ParticleVS.hlsl"), w("ParticlePS.hlsl")))
		return false;
	if (!shader->AddTexture(name))
		return false;
	if (!m_InitBuffers())
		return false;

	for (unsigned int i = 0; i < amount; ++i)
		particles.push_back(Particle());
	return true;
}

bool ParticleGenerator::Init(ID3D11ShaderResourceView* texture, int amount, XMFLOAT3 velosity, float size, float life)
{
	this->velosity = velosity;
	this->amount = amount;
	this->size = size;
	this->life = life;

	shader = new Shader(render);
	shader->AddInputElementDesc("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	shader->AddInputElementDesc("TEXCOORD", DXGI_FORMAT_R32G32_FLOAT);

	if (!shader->CreateShader(w("ParticleVS.hlsl"), w("ParticlePS.hlsl")))
		return false;
	shader->AddTexture(texture);
	if (!m_InitBuffers())
		return false;

	for (unsigned int i = 0; i < amount; ++i)
		particles.push_back(Particle());
	return true;
}


bool ParticleGenerator::m_InitBuffers()
{
	ParticleVertex vertices[4];

	float left = 0;
	float right = size;
	float top = 0;
	float bottom = size;

	vertices[0].pos = XMFLOAT3(top, top, 0.0f);
	vertices[0].tex = XMFLOAT2(0.0f, 0.0f);

	vertices[1].pos = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].tex = XMFLOAT2(1.0f, 1.0f);

	vertices[2].pos = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].tex = XMFLOAT2(0.0f, 1.0f);

	vertices[3].pos = XMFLOAT3(right, top, 0.0f);
	vertices[3].tex = XMFLOAT2(1.0f, 0.0f);

	unsigned long indices[6] =
	{
		0,1,2,
		0,3,1
	};

	m_vertexBuffer = Buffer::CreateVertexBuffer(render->GetDevice(), sizeof(ParticleVertex) * 4, false, &vertices);
	if (!m_vertexBuffer)
		return false;

	m_indexBuffer = Buffer::CreateIndexBuffer(render->GetDevice(), sizeof(unsigned long) * 6, false, &indices);
	if (!m_indexBuffer)
		return false;

	m_constantBuffer = Buffer::CreateConstantBuffer(render->GetDevice(), sizeof(ParticleConstBuf), false);
	if (!m_constantBuffer)
		return false;

	return true;
}


void ParticleGenerator::Update(float frameTime, XMFLOAT3 GameObjectPos, unsigned newParticles, XMFLOAT3 offset)
{
	// Добавляем новые частицы
	for (unsigned int i = 0; i < newParticles; ++i)
	{
		int unusedParticle = FirstUnusedParticle();
		RespawnParticle(particles[unusedParticle], GameObjectPos, offset);
	}

	// Обновляем все частицы
	for (unsigned int i = 0; i < amount; ++i)
	{
		Particle& p = particles[i];
		p.Life -= frameTime; // уменьшаем время жизни
		if (p.Life > 0.0f)
		{	// если частица еще жива, то обновляем её значения

			p.Position.x += p.Velocity.x * frameTime;
			p.Position.y += p.Velocity.y * frameTime;
			p.Position.z += p.Velocity.z * frameTime;
			p.Color.w -= frameTime * 1.0f;
		}

	}
}

int ParticleGenerator::FirstUnusedParticle()
{
	// Поиск по последней использованной частице. Как правило, результат возвращается почти мгновенно
	for (unsigned int i = lastUsedParticle; i < amount; ++i)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	// В противном случае выполняем линейный поиск
	for (unsigned int i = 0; i < lastUsedParticle; ++i)
	{
		if (particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	// Переопределяем первую частицу, если все остальные живы
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, XMFLOAT3 objectPos, XMFLOAT3 offset)
{
	float random = ((rand() % 100) - 50) / 100.0f;
	float rColor = ((rand() % 255) / 255.0f);
	XMVECTOR pos = XMLoadFloat3(&objectPos);

	pos += XMLoadFloat3(&offset);
	pos += SimpleXMVector(random);

	XMStoreFloat4(&(particle.Position), pos);
	particle.Size = size;
	particle.Color = XMFLOAT4(rColor, rColor, rColor, 0.5f);
	particle.Life = life;
	particle.Velocity = XMFLOAT4(velosity.x, velosity.y, velosity.z, 1);
}

// Рендеринг всех частиц
	// Используем аддитивный режим смешивания для придания эффекта свечения
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	///Не забываем сбросить режим смешивания к изначальным настройкам
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


void ParticleGenerator::Draw(CXMMATRIX viewMatrix)
{
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			XMMATRIX pos = XMMatrixIdentity();
			pos *= XMMatrixTranslation(particle.Position.x, particle.Position.y, particle.Position.z);
			XMMATRIX WVP = pos * viewMatrix * render->GetProjection();
			m_RenderBuffers();
			m_SetShaderParameters(WVP, particle.Color);
			//sLog->Debug("Xpos:%lf | Ypos:%lf | Zpos: %lf rad: %lf", particle.Position.x, particle.Position.y, particle.Position.z);
			m_RenderShader();
			
		}
	}
	render->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleGenerator::m_RenderBuffers()
{
	unsigned int stride = sizeof(ParticleVertex);
	unsigned int offset = 0;
	render->GetContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	render->GetContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//render->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleGenerator::m_SetShaderParameters(CXMMATRIX WVP, XMFLOAT4 color)
{
	ParticleConstBuf cb;
	cb.WVP = XMMatrixTranspose(WVP);
	cb.color = color;
	//cb.offset = XMFLOAT4(0,0,0,0);
	render->GetContext()->UpdateSubresource(m_constantBuffer, 0, NULL, &cb, 0, 0);
	render->GetContext()->VSSetConstantBuffers(0, 1, &m_constantBuffer);
}

void ParticleGenerator::m_RenderShader()
{
	shader->Draw();
	render->GetContext()->DrawIndexed(6, 0, 0);
}