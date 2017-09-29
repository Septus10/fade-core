#pragma once

#include <core/definitions.hpp>
#include <core/pimpl.hpp>
#include <graphics/graphics_api.hpp>
#include <vector>

namespace fade { namespace graphics {

class FADE_API renderable
{
public:
    renderable();
    renderable(const renderable& other);
    renderable(std::vector<struct vertex> vertices, std::vector<u32> indices);
    virtual ~renderable();
    
    void setup_vertex_buffer(std::vector<struct vertex> vertices, std::vector<u32> indices);

    virtual void draw();
private:
    FADE_MAKE_PIMPL
};

} }
