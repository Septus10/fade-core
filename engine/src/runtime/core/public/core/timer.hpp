#pragma once

#include <core/core_api.hpp>
#include <core/pimpl.hpp>

#include <memory>

namespace fade {

class FADE_API timer
{
public:
    timer();
    ~timer();

	void start();

	void reset();
	
	void stop();

	void pause();

	void resume();

	double elapsed() const;

private:
    FADE_MAKE_PIMPL;
};


}
