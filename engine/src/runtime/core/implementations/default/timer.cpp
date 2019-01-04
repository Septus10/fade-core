#include <Core/Timer.hpp>

namespace Fade {

CTimer::CTimer()
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
	m_Start = m_Pause = m_Resume = hrc::now();
	m_HasPaused = false;
	m_IsStopped = false;
}

void CTimer::Stop()
{
	m_Stop = hrc::now();
	m_IsStopped = true;
}

void CTimer::Pause()
{
	m_Pause = hrc::now();
	m_HasPaused = true;
	m_Resume = hrc::now();
}

void CTimer::Resume()
{
	m_Resume = hrc::now();
}

double CTimer::Elapsed() const
{
	std::chrono::duration<double> duration;
	if (m_HasPaused)
	{
		if (m_IsStopped)
		{
			duration = m_Stop - m_Resume + (m_Pause - m_Start);
		}
		else
		{
			duration = hrc::now() - m_Resume + (m_Pause - m_Start);
		}

        return duration.count();
	}		

	if (m_IsStopped)
	{
		duration = m_Stop - m_Start;
	}
	else
	{
		duration = hrc::now() - m_Start;
	}

	return duration.count();
}

}
