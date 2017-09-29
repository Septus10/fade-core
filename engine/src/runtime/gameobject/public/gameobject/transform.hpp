#pragma once

#include <glm.hpp>
#include <gameobject/gameobject_api.hpp>

namespace fade {
   
class FADE_API transform
{
public:
    transform();
    transform(const transform& transform);
    transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    const glm::vec3& get_position() const;
    const glm::vec3& get_rotation() const;
    const glm::vec3& get_scale()    const;

    const glm::mat4& get_matrix()   const;

    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_scale(const glm::vec3& scale);

    void set(const transform& transform);

private:
    glm::vec3 position_, rotation_, scale_;
    glm::mat4 mat_translation_, mat_rotation_, mat_scale_;

};

}
