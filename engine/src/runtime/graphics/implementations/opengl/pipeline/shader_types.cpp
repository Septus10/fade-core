#pragma once

#include <core/definitions.hpp>
#include <graphics/pipeline/shader_types.hpp>

#define GLEW_STATIC

#include <gl/glew.h>

namespace fade { namespace graphics { namespace pipeline {

// for all uniform types see: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml
SHADER_DATA_TYPE convert_library_type(u32 library_type)
{
    if (library_type == GL_BOOL)
    {
        return SHADER_DATA_TYPE::SDT_BOOL;
    }

    if (library_type == GL_INT)
    {
        return SHADER_DATA_TYPE::SDT_INT;
    }

    if (library_type == GL_FLOAT)
    {
        return SHADER_DATA_TYPE::SDT_FLOAT;
    }

    if (library_type == GL_SAMPLER_1D)
    {
        return SHADER_DATA_TYPE::SDT_SAMPLER1D;
    }

    if (library_type == GL_SAMPLER_2D)
    {
        return SHADER_DATA_TYPE::SDT_SAMPLER2D;
    }

    if (library_type == GL_SAMPLER_3D)
    {
        return SHADER_DATA_TYPE::SDT_SAMPLER3D;
    }

    if (library_type == GL_SAMPLER_CUBE)
    {
        return SHADER_DATA_TYPE::SDT_SAMPLERCUBE;
    }

    if (library_type == GL_FLOAT_VEC2)
    {
        return SHADER_DATA_TYPE::SDT_VEC2;
    }

    if (library_type == GL_FLOAT_VEC3)
    {
        return SHADER_DATA_TYPE::SDT_VEC3;
    }

    if (library_type == GL_FLOAT_VEC4)
    {
        return SHADER_DATA_TYPE::SDT_VEC4;
    }

    if (library_type == GL_FLOAT_MAT2)
    {
        return SHADER_DATA_TYPE::SDT_MAT2;
    }

    if (library_type == GL_FLOAT_MAT3)
    {
        return SHADER_DATA_TYPE::SDT_MAT3;
    }

    if (library_type == GL_FLOAT_MAT4)
    {
        return SHADER_DATA_TYPE::SDT_MAT4;
    }

    return SHADER_DATA_TYPE::SDT_UNKNOWN;
}

uniform_value::uniform_value()
{
    type_ = SDT_UNKNOWN;
    i_val_ = 0;
}

uniform_value::uniform_value(SHADER_DATA_TYPE type)
{
    type_ = type;
    i_val_ = 0;
}

} } }

