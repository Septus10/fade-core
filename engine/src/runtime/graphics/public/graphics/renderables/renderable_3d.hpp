#pragma once

#include <graphics/renderables/renderable.hpp>
#include <gameobject/gameobject.hpp>

namespace fade { namespace graphics
{
    
class FADE_API renderable_3d : public renderable, gameobject
{
public:
    renderable_3d();
    renderable_3d(const renderable_3d& other);

    void draw() override;
};

} }