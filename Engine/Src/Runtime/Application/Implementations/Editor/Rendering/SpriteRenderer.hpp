#pragma once

#include <Core/definitions.hpp>

namespace Fade { namespace Rendering {

static const f32 sg_SpriteVertices[] = {
	// Top left
	-1.0f, 1.0f,
	// Top right
	1.0f , 1.0f,
	// Bottom right
	1.0f ,-1.0f,
	// Bottom left
	-1.0f,-1.0f
};

static const i32 sg_SpriteIndices[] = {
	0, 1, 2,
	2, 3, 0
};

} }