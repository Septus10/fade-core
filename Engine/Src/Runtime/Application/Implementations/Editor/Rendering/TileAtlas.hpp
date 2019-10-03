#pragma once

#include <Core/Definitions.hpp>
#include "Texture.hpp"

namespace Fade { namespace Rendering {

class CTileAtlas
{
public:
	CTileAtlas() {}
	CTileAtlas(CTexture a_Texture, u32 a_TileDimensions, u32 a_Padding) :
		m_Texture(a_Texture),
		m_TileDimensions(a_TileDimensions),
		m_Padding(a_Padding)
	{ }
	
	const CTexture& GetTexture() const
	{
		return m_Texture;
	}

private:
	CTexture m_Texture;

	u32 m_TileDimensions;
	u32 m_Padding;
};

} }