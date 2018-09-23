#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Pimpl.hpp>

#include <chrono>


namespace Fade {

class FADE_CORE_API CTimer
{
public:
	CTimer();
    ~CTimer();

	void Start();

	void Reset();
	
	void Stop();

	void Pause();

	void Resume();

	double Elapsed() const;

private:
	using hrc	= std::chrono::high_resolution_clock;
	using ms	= std::chrono::milliseconds;

	hrc::time_point m_Start, m_Pause, m_Resume, m_Stop;
	bool m_HasPaused;
	bool m_IsStopped;
};


}
