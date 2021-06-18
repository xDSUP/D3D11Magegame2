#include "stdafx.h"
#include "Fps.h"
#include "timeapi.h"

using namespace D3D11Framework;

void Fps::Init()
{
	m_fps = 0;
	m_count = 0;
	m_startTime = timeGetTime();
}

void Fps::Frame()
{
	m_count++;

	if(timeGetTime() >= (m_startTime + 1000))
	{
		m_fps = m_count;
		m_count = 0;

		m_startTime = timeGetTime();
	}
}

int Fps::GetFps()
{
	return m_fps;
}