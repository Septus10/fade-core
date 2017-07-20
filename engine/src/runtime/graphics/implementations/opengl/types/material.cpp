#include <graphics/types/material.hpp>

#include <core/definitions.hpp>
#include <algorithm>

namespace fade { namespace graphics {

material::material() :
    program_(nullptr)
{
}

material::material(pipeline::shader_program* program) :
    program_(program)
{
    
}

material::~material()
{
    
}

} }
