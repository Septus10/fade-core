#pragma once

#include <vector>
#include <graphics/pipeline/shader_program.hpp>
#include <graphics/types/material.hpp>

namespace fade { namespace graphics {
    
struct render_batch
{
    std::vector<class renderable*>  renderables_;
    material*                       material_;
};

} }

