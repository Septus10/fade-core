#pragma once

#include <graphics/renderables/renderable.hpp>

namespace fade { namespace graphics
{
    
class FADE_API renderable_3d : public renderable
{
public:
    renderable_3d();
    renderable_3d(const renderable_3d& lhs);
};

} }