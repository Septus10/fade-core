#pragma once

#include <Core/definitions.hpp>
#include <Core/Containers/String.hpp>
#include <Rendering/Pipeline/ShaderProgram.hpp>
#include <Rendering/Texture.hpp>

namespace Fade {

class CLandscapeChunk
{
public:
	CLandscapeChunk() {};
	CLandscapeChunk(fstl::String a_Path);
	CLandscapeChunk(u32 a_ChunkWidth, u32 a_ChunkHeight);

	void Create(u32 a_ChunkWidth, u32 a_ChunkHeight);

	void SetTileAtlas(Rendering::CTexture a_Texture);
	void SetTilemap(Rendering::CTexture a_Texture);

	void Draw(glm::mat4 a_MVP);

private:	
	static Rendering::Pipeline::CShaderProgram sm_Landscape2DProgram;
	static void LoadLandscapeProgram();

	// hardcoded tile width and height for now
	static u32 sm_TileWidth;
	static u32 sm_TileHeight;

	Rendering::CTexture m_Tilemap, m_TileAtlas;
	u32 m_MapTileWidth, m_MapTileHeight;

	u32 m_ChunkWidth, m_ChunkHeight, m_NumIndices;
	u32 m_VAO;
};

class CLandscape2D
{
public:
	CLandscape2D() = delete;
	CLandscape2D(fstl::String a_Path);
	CLandscape2D(u32 a_Width, u32 a_Height, u32 a_ChunkWidth, u32 a_ChunkHeight);

	void Draw(glm::mat4 a_MVP);
private:
	CLandscapeChunk** m_Chunks;

	u32 m_Width, m_Height;
	u32 m_NumChunks;
};

}