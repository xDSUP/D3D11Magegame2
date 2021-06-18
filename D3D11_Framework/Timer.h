#pragma once

namespace D3D11Framework
{
//------------------------------------------------------------------

	class Timer
	{
	public:
		bool Init();
		void Frame();

		float GetFrameTime();
		INT64 GetCurTime();

	private:
		INT64 m_frequency;
		INT64 m_startTime;
		float m_ticksPerMs;
		float m_frameTime;
	};

//------------------------------------------------------------------
}