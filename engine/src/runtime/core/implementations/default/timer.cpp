#include <Core/Timer.hpp>

namespace Fade {

class CTimer::CImpl
{
	using HRC	= std::chrono::high_resolution_clock;
	using MS	= std::chrono::milliseconds;

	HRC::time_point m_Start, m_Pause, m_Resume, m_Stop;	

public:
	std::chrono::duration<double> GetDuration(bool a_HasPaused, bool a_IsStopped) const
	{
		std::chrono::duration<double> Duration;
		if (a_HasPaused)
		{
			if (a_IsStopped)
			{
				Duration = m_Stop - m_Resume + (m_Pause - m_Start);
			}
			else
			{
				Duration = HRC::now() - m_Resume + (m_Pause - m_Start);
			}

			return Duration;
		}		

		if (a_IsStopped)
		{
			Duration = m_Stop - m_Start;
		}
		else
		{
			Duration = HRC::now() - m_Start;
		}

		return Duration;
	}

	void Reset()
	{
		m_Start = m_Pause = m_Resume = HRC::now();
	}

	void Stop()
	{
		m_Stop = HRC::now();
	}

	void Pause()
	{
		m_Pause = HRC::now();
		m_Resume = HRC::now();
	}

	void Resume()
	{
		m_Resume = HRC::now();
	}
};

CTimer::CTimer() :
	FADE_INIT_PIMPL(CTimer)
{
    m_HasPaused = false;
    m_IsStopped = false;
}

CTimer::~CTimer()
{
}

void CTimer::Start()
{
	Reset();
}

void CTimer::Reset()
{
	m_Impl->Reset();
	m_HasPaused = false;
	m_IsStopped = false;
}

void CTimer::Stop()
{
	m_Impl->Stop();
	m_IsStopped = true;
}

void CTimer::Pause()
{
	m_Impl->Pause();
	m_HasPaused = true;
}

void CTimer::Resume()
{
	m_Impl->Resume();
}

double CTimer::Elapsed() const
{
	return m_Impl->GetDuration(m_HasPaused, m_IsStopped).count();
}

}
