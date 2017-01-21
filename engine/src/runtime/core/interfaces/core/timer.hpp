#pragma once

#include <core/core_api.hpp>

#include <memory>

namespace fade {

class FADE_API timer abstract
{
public:
	virtual ~timer() = default;

	virtual void start() = 0;

	virtual void reset() = 0;
	
	virtual void stop() = 0;

	virtual void pause() = 0;

	virtual void resume() = 0;

	virtual double elapsed() const = 0;
};

std::unique_ptr<timer> FADE_API get_timer();

}