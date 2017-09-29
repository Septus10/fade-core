#pragma once

#include <core/pimpl.hpp>
#include <gameobject/transform.hpp>
#include <gameobject/gameobject_api.hpp>

namespace fade {

class FADE_API gameobject
{
public:
    /*
     * default ctor
     */
    gameobject();

    /*
     * copy constructor
     */
    gameobject(const gameobject& rhs);
    
    virtual ~gameobject();

    const transform&  get_transform() const;
    const glm::mat4&  get_matrix()    const;
    const glm::vec3&  get_position()  const;
    const glm::vec3&  get_rotation()  const;
    const glm::vec3&  get_scale()     const;

    void set_transform(const transform& transform);
    void set_position(const glm::vec3& position);
    void set_rotation(const glm::vec3& rotation);
    void set_scale(const glm::vec3& scale);

private:
    FADE_MAKE_PIMPL
    
};

}