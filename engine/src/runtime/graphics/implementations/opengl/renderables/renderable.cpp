#include <graphics/renderables/renderable.hpp>
#include <graphics/types/vertex.hpp>

#define GLEW_STATIC

#include <gl/glew.h>
#include <gl/gl.h>

namespace fade { namespace graphics {
    
class renderable::impl
{
public:
    u32 VBO_;
    u32 EBO_;
    u32 VAO_;
    u32 num_vertices_;
};

renderable::renderable() :
    FADE_INIT_PIMPL(renderable)
{
}

renderable::renderable(const renderable& lhs) :
    FADE_INIT_PIMPL(renderable)
{
    impl_->VBO_ = lhs.impl_->VBO_;
    impl_->EBO_ = lhs.impl_->EBO_;
    impl_->VAO_ = lhs.impl_->VAO_;
    impl_->num_vertices_ = lhs.impl_->num_vertices_;
}

renderable::renderable(std::vector<vertex> vertices, std::vector<u32> indices) :
    FADE_INIT_PIMPL(renderable)
{
    setup_vertex_buffer(vertices, indices);
}

renderable::~renderable()
{

}

void renderable::setup_vertex_buffer(std::vector<vertex> vertices, std::vector<u32> indices)
{
    // set up VAO
    glGenVertexArrays(1, &impl_->VAO_);

    // set up VBO_
    glGenBuffers(1, &impl_->VBO_);
    glGenBuffers(1, &impl_->EBO_);

    glBindVertexArray(impl_->VAO_);
    //GL_STATIC_DRAW:  the data will most likely not change at all or very rarely.
    //GL_DYNAMIC_DRAW: the data is likely to change a lot.
    //GL_STREAM_DRAW:  the data will change every time it is drawn.
    glBindBuffer(GL_ARRAY_BUFFER, impl_->VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl_->EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    // set vertex position attribute pointer
    i32 stride = sizeof(vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(vertex, position)));
    glEnableVertexAttribArray(0);

    // set normal attribute pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(vertex, normal)));
    glEnableVertexAttribArray(1);

    // set UV attribute pointer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(vertex, uv)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    impl_->num_vertices_ = u32(indices.size());
}

void renderable::draw()
{
    glBindVertexArray(impl_->VAO_);
    glDrawElements(GL_TRIANGLES, impl_->num_vertices_, GL_UNSIGNED_INT, nullptr);
}

} }
