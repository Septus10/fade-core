#include <graphics/pipeline/shader_program.hpp>

#define GLEW_STATIC

#include <gl/glew.h>

#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>

#include <graphics/pipeline/shader_types.hpp>
#include <graphics/types/material.hpp>

namespace fs = std::experimental::filesystem;

namespace fade
{
   
namespace graphics
{
    
namespace pipeline
{

class shader_program::impl
{
public:
    u32                             shader_program_;
};

shader_program::shader_program() :
    FADE_INIT_PIMPL(shader_program)
{
}

shader_program::shader_program(std::string file_path) :
    FADE_INIT_PIMPL(shader_program)
{
    create(file_path);
}

shader_program::~shader_program()
{
}

bool compile_shader(i32 program, i32 type, const char* source, std::vector<u32>& shaders)
{
    u32 shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "shader_program::compile_shader - Error: " << infoLog << std::endl;
        return false;
    }

    shaders.push_back(shader);
    glAttachShader(program, shader);

    return true;
}

bool shader_program::create(std::string directory_path)
{
    // create the shader program
    impl_->shader_program_ = glCreateProgram();

    if (!fs::exists(directory_path))
    {
        return false;
    }

    std::vector<u32> shaders;
    for (auto& p : fs::directory_iterator(directory_path))
    {
        // get the file path
        fs::path path(p);
        // get the file extension
        std::string extension = fs::path(p).string();
        extension = extension.substr(extension.find_last_of('.') + 1);
        std::fstream file(path);

        u32 shader_type;
        if (!file.is_open())
        {
            return false;
        }

        // derive shader type from file extension
        if (extension.find("vs") != std::string::npos)
        {
            shader_type = GL_VERTEX_SHADER;
        }
        else if (extension.find("fs") != std::string::npos)
        {
            shader_type = GL_FRAGMENT_SHADER;
        }
        else if (extension.find("gs") != std::string::npos)
        {
            shader_type = GL_GEOMETRY_SHADER;
        }
        else if (extension.find("cs") != std::string::npos)
        {
            shader_type = GL_COMPUTE_SHADER;
        }
        else
        {
            return false;
        }

        // read contents of file into string
        std::string file_contents;
        file.seekg(0, std::ios::end);
        file_contents.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        file_contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        // compile the shader
        if (!compile_shader(impl_->shader_program_, shader_type, file_contents.c_str(), shaders))
        {
            return false;
        }
    }

    glLinkProgram(impl_->shader_program_);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(impl_->shader_program_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(impl_->shader_program_, 512, NULL, infoLog);
        std::cout << "shader_program::create - Error: " << infoLog << std::endl;
        return false;
    }

    i32 uniform_count, attribute_count;
    glGetProgramiv(impl_->shader_program_, GL_ACTIVE_UNIFORMS, &uniform_count);
    glGetProgramiv(impl_->shader_program_, GL_ACTIVE_ATTRIBUTES, &attribute_count);

    uniforms_.resize(uniform_count);
    attributes_.resize(attribute_count);

    char buffer[128];
    u32 vsize = u32(uniforms_.size());
    for (u32 i = 0; i < vsize; i++)
    {
        GLenum gl_type;
        glGetActiveUniform(impl_->shader_program_, i, sizeof(buffer), nullptr, &uniforms_[i].size_, &gl_type, buffer);
        i32 uniform_location = glGetUniformLocation(impl_->shader_program_, buffer);
        uniforms_[i].name_ = std::string(buffer);
        uniforms_[i].type_ = convert_library_type(gl_type);
        uniforms_[i].location_ = uniform_location;
    }

    vsize = u32(attributes_.size());
    for (usize i = 0; i < vsize; i++)
    {
        GLenum gl_type;
        glGetActiveAttrib(impl_->shader_program_, i, sizeof(buffer), nullptr, &attributes_[i].size_, &gl_type, buffer);
        i32 attrib_location = glGetAttribLocation(impl_->shader_program_, buffer);
        attributes_[i].name_ = std::string(buffer);
        attributes_[i].type_ = convert_library_type(gl_type);
        attributes_[i].location_ = attrib_location;
    }

    for (auto i : shaders)
    {
        glDeleteShader(i);
    }

    return true;
}

void shader_program::use()
{
    glUseProgram(impl_->shader_program_);
}

void shader_program::stop()
{
    glUseProgram(0);
}

void shader_program::setup_uniforms(material* mat)
{
    for (auto uniform: uniforms_)
    {
        switch(uniform.type_)
        {
        case SDT_BOOL:
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].b_val_);
            break;
        case SDT_INT:
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].i_val_);
            break;
        case SDT_FLOAT:
            glUniform1f(uniform.location_, mat->uniforms_[uniform.name_].f_val_);
            break;
        case SDT_SAMPLER1D: 
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].i_val_);
            break;
        case SDT_SAMPLER2D: 
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].i_val_);
            break;
        case SDT_SAMPLER3D: 
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].i_val_);
            break;
        case SDT_SAMPLERCUBE: 
            glUniform1i(uniform.location_, mat->uniforms_[uniform.name_].i_val_);
            break;
        case SDT_VEC2: 
            glUniform2fv(uniform.location_, 2, &mat->uniforms_[uniform.name_].v2_val_.x);
            break;
        case SDT_VEC3:
            glUniform3fv(uniform.location_, 3, &mat->uniforms_[uniform.name_].v3_val_.x);
            break;
        case SDT_VEC4:
            glUniform4fv(uniform.location_, 4, &mat->uniforms_[uniform.name_].v4_val_.x);
            break;
        case SDT_MAT2:
            glUniformMatrix2fv(uniform.location_, 4, false, &mat->uniforms_[uniform.name_].m2_val_[0][0]);
            break;
        case SDT_MAT3: 
            glUniformMatrix2fv(uniform.location_, 9, false, &mat->uniforms_[uniform.name_].m3_val_[0][0]);
            break;
        case SDT_MAT4: 
            glUniformMatrix2fv(uniform.location_, 12, false, &mat->uniforms_[uniform.name_].m4_val_[0][0]);
            break;
        case SDT_UNKNOWN: 
            break;
        }
    }
}

std::vector<uniform> shader_program::get_uniforms() const
{
    return uniforms_;
}

std::vector<vertex_attribute> shader_program::get_attributes() const
{
    return attributes_;
}

} } }
