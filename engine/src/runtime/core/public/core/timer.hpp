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
	FADE_MAKE_PIMPL;
	
	bool m_HasPaused;
	bool m_IsStopped;
};


}
