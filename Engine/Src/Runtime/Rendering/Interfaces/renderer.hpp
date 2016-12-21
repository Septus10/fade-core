#pragma once

#include <rendering_api.hpp>

namespace fade
{

namespace rendering
{
	
class FADE_API renderer abstract
{
public:
	virtual ~renderer() = default;
	virtual void foo() = 0;

private:

};

}

}