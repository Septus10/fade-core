#include <gameobject/transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <matrix.hpp>
#include <gtx/transform.hpp>

namespace fade {
//=========================================================//
transform::transform() :
    position_(0, 0, 0),
    rotation_(0, 0, 0),
    scale_(0, 0, 0)
{
}
//=========================================================//
transform::transform(const transform& transform) :
    position_(transform.position_),
    rotation_(transform.rotation_),
    scale_(transform.scale_)
{
}
//=========================================================//
transform::transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) :
    position_(position),
    rotation_(rotation),
    scale_(scale)
{
}
//=========================================================//
const glm::vec3& transform::get_position() const
{
    return position_;
}
//=========================================================//
const glm::vec3& transform::get_rotation() const
{
    return rotation_;
}
//=========================================================//
const glm::vec3& transform::get_scale() const
{
    return scale_;
}
//=========================================================//
const glm::mat4& transform::get_matrix() const
{
    return (mat_translation_ * mat_rotation_ * mat_scale_);
}
//=========================================================//
void transform::set_position(const glm::vec3& position)
{
    position_ = position;
    mat_translation_ = glm::translate(position);
}
//=========================================================//
void transform::set_rotation(const glm::vec3& rotation)
{
    rotation_ = rotation;
    glm::mat4 x_rot, y_rot, z_rot;
    glm::rotate(x_rot, rotation.x, glm::vec3(1, 0, 0));
    glm::rotate(y_rot, rotation.y, glm::vec3(0, 1, 0));
    glm::rotate(z_rot, rotation.z, glm::vec3(0, 0, 1));
    mat_rotation_ = z_rot * y_rot * x_rot;
}
//=========================================================//
void transform::set_scale(const glm::vec3& scale)
{
    scale_ = scale;
    mat_scale_ = glm::scale(scale);
}
//=========================================================//
void transform::set(const transform& transform)
{
    position_ = transform.position_;
    rotation_ = transform.rotation_;
    scale_ = transform.scale_;
}
//=========================================================//
}
