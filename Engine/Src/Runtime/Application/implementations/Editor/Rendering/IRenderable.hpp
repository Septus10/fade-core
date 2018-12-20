#pragma once

#include <Core/definitions.hpp>

namespace Fade {

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual void Draw() = 0;
};

class IRenderable2D : public IRenderable
{
public:
	virtual ~IRenderable2D() = default;

	virtual void Draw() = 0;

private:
	u32 m_LayerIndex;
};

}