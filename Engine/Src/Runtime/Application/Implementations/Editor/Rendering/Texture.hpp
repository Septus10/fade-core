#pragma once

#include <Core/Containers/String.hpp>
#include <Core/definitions.hpp>
#include <glm.hpp>

namespace Fade {
namespace Rendering {

class CTexture
{
public:
	CTexture() {}
	CTexture(fstl::String a_TexturePath);

	bool LoadTexture(fstl::String a_TexturePath);

	u32 GetTextureID() const { return m_TextureID; }

	i32 GetWidth() const { return m_Width; }
	i32 GetHeight() const { return m_Height; }

protected:
	Fade::uc8* getImageDataFromPath(fstl::String a_Filepath, Fade::u32& a_OutFormat);
	void freeImageData(Fade::uc8* a_Data);

	Fade::u32 m_TextureID;

 	Fade::i32 m_Width, m_Height;
};

class CTileAtlas : public CTexture
{
public:
	CTileAtlas() {}
	CTileAtlas(fstl::String a_TexturePath, glm::vec2 a_TileDimensions, u32 a_PaddingWith);

	u32 GetTileWidth();
	u32 GetTileHeight();

	glm::vec2 GetTileDimensions() { return m_TileDimensions; }

	f32 GetPaddingWidth() { return m_PaddingWidth; }

private:
	glm::vec2 m_TileDimensions;
	f32 m_PaddingWidth;
};

} }