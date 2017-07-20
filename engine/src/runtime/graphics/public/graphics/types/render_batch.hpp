#pragma once

#include <vector>
#include <graphics/pipeline/shader_program.hpp>

namespace fade { namespace graphics {
    
struct render_batch
{
    std::vector<class renderable>   renderables_;
    pipeline::shader_program*       shader_program_;
};

} }

