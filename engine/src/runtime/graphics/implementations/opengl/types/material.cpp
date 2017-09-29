#include <graphics/types/material.hpp>

#include <graphics/pipeline/shader_types.hpp>

#include <core/definitions.hpp>
#include <algorithm>

namespace fade { namespace graphics {

material::material(pipeline::shader_program* program)
{
    set_shader_program(program);
}

material::~material()
{
    
}

void material::set_shader_program(pipeline::shader_program* program)
{
    program_ = program;

    for (auto uniform : program_->get_uniforms())
    {
        uniforms_[uniform.name_] = pipeline::uniform_value(uniform.type_);
    }
}

pipeline::shader_program* material::get_shader_program() const
{
    return program_;
}

} }
