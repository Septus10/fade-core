#pragma once

#include <graphics/graphics_api.hpp>
#include <core/pimpl.hpp>

namespace fade
{

namespace graphics
{

namespace pipeline
{
    
class FADE_API shader_manager
{
public:
    shader_manager();
    ~shader_manager();

private:
    FADE_MAKE_PIMPL
};

}

}

}