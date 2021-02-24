#pragma once

#define FADE_USE_GLM 1

#if (FADE_USE_GLM)
#include <Core/Math/glm/glm.hpp>
#else
#include <Core/Math/Vector2.hpp>
//#include <Core/Math/Vector3.hpp>
#endif

#if (FADE_USE_GLM)

namespace Fade { namespace Math {

	using Vec2 = glm::vec2;
	using iVec2 = glm::ivec2;
	using uVec2 = glm::uvec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;

	using Mat2 = glm::mat2;
	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;

} }

#endif