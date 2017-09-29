#pragma once

#include <graphics/graphics_api.hpp>
#include <graphics/renderer.hpp>
#include <core/pimpl.hpp>
#include <vector>
#include <core/definitions.hpp>
#include <unordered_map>

namespace fade { namespace graphics { 

class material;
    
namespace pipeline {

struct shader_uniform;
class FADE_API shader_program
{
public:
    shader_program();
    shader_program(std::string file_path);
    ~shader_program();

    bool create(std::string file_path);

    void use();
    void stop();

    void setup_uniforms(material* mat);

    std::vector<struct uniform>             get_uniforms() const;
    std::vector<struct vertex_attribute>    get_attributes() const;

private:
    FADE_MAKE_PIMPL

    std::vector<struct uniform>            uniforms_;
    std::vector<struct vertex_attribute>   attributes_;
};

} } }
