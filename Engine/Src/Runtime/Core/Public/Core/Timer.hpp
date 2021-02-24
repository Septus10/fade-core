#pragma once

#include <Core/CoreApi.hpp>
#include <chrono>

namespace Fade {

class CTimer
{
	using HRC = std::chrono::high_resolution_clock;
	using MS = std::chrono::milliseconds;
	HRC::time_point m_Start, m_Stop;

	std::chrono::duration<double> m_Duration;

	bool m_IsPaused;
	bool m_IsStopped;

public:
	CTimer() 
		: m_IsPaused(false) 
		, m_IsStopped(false)
	{}

	void Start()
	{
		Reset();
		m_Start = HRC::now();
	}

	void Reset() 
	{
		m_Duration = std::chrono::duration<double>::zero();
	}
	
	void Stop()
	{
		m_IsStopped = true;
		m_Stop = HRC::now();
	}

	void Pause()
	{
		if (!m_IsPaused)
		{
			m_Duration += HRC::now() - m_Start;
			m_IsPaused = true;
		}		
	}

	void Resume()
	{
		if (m_IsPaused)
		{
			m_Start = HRC::now();
		}
		m_IsPaused = false;
	}

	template <typename ReturnType>
	ReturnType Elapsed()
	{
		if (!m_IsStopped && !m_IsPaused)
		{
			m_Duration += (HRC::now() - m_Start);
		}		
		std::chrono::duration<ReturnType> Duration(m_Duration);
		return Duration.count();
	}
};

}
