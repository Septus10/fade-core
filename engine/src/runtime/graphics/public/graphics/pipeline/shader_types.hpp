#pragma once

#include <graphics/graphics_api.hpp>
#include <string>
#include <core/definitions.hpp>
#include <glm.hpp>

// credits for this go to Joey de Vries (https://github.com/JoeyDeVries/Cell)

namespace fade { namespace graphics { namespace pipeline {
    
enum SHADER_DATA_TYPE
{
    SDT_BOOL,
    SDT_INT,
    SDT_FLOAT,
    SDT_SAMPLER1D,
    SDT_SAMPLER2D,
    SDT_SAMPLER3D,
    SDT_SAMPLERCUBE,
    SDT_VEC2,
    SDT_VEC3,
    SDT_VEC4,
    SDT_MAT2,
    SDT_MAT3,
    SDT_MAT4,
    SDT_UNKNOWN
};

SHADER_DATA_TYPE FADE_API convert_library_type(u32 library_type);

struct uniform
{
    SHADER_DATA_TYPE    type_;
    std::string         name_;
    i32                 size_;
    u32                 location_;
};

struct uniform_value
{
    SHADER_DATA_TYPE    type_;
    union
    {
        bool        b_val_;
        int         i_val_;
        float       f_val_;

        glm::vec2   v2_val_;
        glm::vec3   v3_val_;
        glm::vec4   v4_val_;
        glm::mat2   m2_val_;
        glm::mat3   m3_val_;
        glm::mat4   m4_val_;
    };
};

struct uniform_value_sampler
{
    SHADER_DATA_TYPE    type_;
    u32                 unit_;
    union
    {
        class texture*      texture_;
        class texture_cube* texture_cube_;
    };
};

struct vertex_attribute
{
    SHADER_DATA_TYPE    type_;
    std::string         name_;
    i32                 size_;
    u32                 location_;
};

} } }