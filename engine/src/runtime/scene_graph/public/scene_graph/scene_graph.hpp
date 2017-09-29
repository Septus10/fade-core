#pragma once

#include <scene_graph/scene_graph_api.hpp>
#include <core/pimpl.hpp>

namespace fade {
    
class FADE_API scene_graph
{
public:
    scene_graph();
    ~scene_graph();

private:
    FADE_MAKE_PIMPL
};

}