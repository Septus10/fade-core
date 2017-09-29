#include <gameobject/gameobject.hpp>

namespace fade {

class gameobject::impl
{
public:
    transform transform_;
};

//=========================================================//
gameobject::gameobject() :
    FADE_INIT_PIMPL(gameobject)
{
}
//=========================================================//
gameobject::gameobject(const gameobject& rhs)
{
}
//=========================================================//
gameobject::~gameobject()
{
}
//=========================================================//
const transform& gameobject::get_transform() const
{
    return impl_->transform_;
}
//=========================================================//
const glm::mat4& gameobject::get_matrix() const
{
    return impl_->transform_.get_matrix();
}
//=========================================================//
const glm::vec3& gameobject::get_position() const
{
    return impl_->transform_.get_position();
}
//=========================================================//
const glm::vec3& gameobject::get_rotation() const
{
    return impl_->transform_.get_rotation();
}
//=========================================================//
const glm::vec3& gameobject::get_scale() const
{
    return impl_->transform_.get_scale();
}
//=========================================================//
void gameobject::set_transform(const transform& transform)
{
    impl_->transform_.set(transform);
}
//=========================================================//
void gameobject::set_position(const glm::vec3& position)
{
    impl_->transform_.set_position(position);
}
//=========================================================//
void gameobject::set_rotation(const glm::vec3& rotation)
{
    impl_->transform_.set_rotation(rotation);
}
//=========================================================//
void gameobject::set_scale(const glm::vec3& scale)
{
    impl_->transform_.set_scale(scale);
}
//=========================================================//
}
