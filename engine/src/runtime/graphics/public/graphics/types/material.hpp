#pragma once

#include <core/pimpl.hpp>
#include <graphics/pipeline/shader_program.hpp>
#include <graphics/pipeline/shader_types.hpp>
#include <unordered_map>

namespace fade { namespace graphics {
    
class material
{
public:
    material(pipeline::shader_program* program);
    ~material();
    
    void set_shader_program(pipeline::shader_program* program);
    pipeline::shader_program* get_shader_program() const;

    std::unordered_map<std::string, pipeline::uniform_value>            uniforms_;
    std::unordered_map<std::string, pipeline::uniform_value_sampler>    sampler_uniforms_;
private:
    pipeline::shader_program* program_;
    
};

} }
