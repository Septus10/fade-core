#pragma once

#include <Core/Definitions.hpp>
#include "Texture.hpp"

namespace Fade { namespace Rendering {

class CTileAtlas
{
public:
	CTileAtlas() {}
	CTileAtlas(CTexture a_Texture, float a_TileDimensions) :
		m_Texture(a_Texture),
		m_TileDimensions(a_TileDimensions)
	{ }
	
	const CTexture& GetTexture() const
	{
		return m_Texture;
	}

	glm::vec2 GetTileDimensions() const
	{
		return m_TileDimensions;
	}

private:
	CTexture m_Texture;

	glm::vec2 m_TileDimensions;
};

} }