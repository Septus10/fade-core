#include "Landscape2D.hpp"

#define GLEW_STATIC 1

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

namespace Fade {

static bool isInitialized = false;
void CLandscapeChunk::LoadLandscapeProgram()
{
	if (!isInitialized)
	{
		sm_Landscape2DProgram.LoadShaderProgram(".\\Shaders\\Landscape2D\\");
		isInitialized = true;
	}	
}

Rendering::Pipeline::CShaderProgram CLandscapeChunk::sm_Landscape2DProgram;

u32 CLandscapeChunk::sm_TileWidth = 32;
u32 CLandscapeChunk::sm_TileHeight = 32;

CLandscape2D::CLandscape2D(fstl::String a_Path)
{

}

CLandscape2D::CLandscape2D(u32 a_Width, u32 a_Height, u32 a_ChunkWidth, u32 a_ChunkHeight)
{

}

void CLandscape2D::Draw(glm::mat4 a_MVP)
{
	for (u32 i = 0; i < m_NumChunks; ++i)
	{
		m_Chunks[i]->Draw(a_MVP);
	}
}

CLandscapeChunk::CLandscapeChunk(fstl::String a_Path)
{

}

CLandscapeChunk::CLandscapeChunk(u32 a_ChunkWidth, u32 a_ChunkHeight) :
	m_ChunkWidth(a_ChunkWidth),
	m_ChunkHeight(a_ChunkHeight)
{
	
}

struct SVertex2D
{
	SVertex2D(glm::vec2 a_Position, glm::vec2 a_UV, glm::vec2 a_TilemapUV) :
		m_Position(a_Position),
		m_UV(a_UV),
		m_TilemapUV(a_TilemapUV)
	{}

	glm::vec2	m_Position;
	glm::vec2	m_UV;
	glm::vec2	m_TilemapUV;
};

inline int Clamp(int a, int min, int max)
{
	return a < min ? min : a > max ? max : a;
}

void CLandscapeChunk::Create(u32 a_ChunkWidth, u32 a_ChunkHeight)
{
	m_ChunkWidth = a_ChunkWidth;
	m_ChunkHeight = a_ChunkHeight;

	TArray<SVertex2D>	vertices;
	TArray<u32>			indices;

	/*
	u32 horizontalVertices = m_ChunkWidth + 1;
	u32 verticalVertices = m_ChunkHeight + 1;

	//generate vertices
	for (u32 y = 0; y < verticalVertices; ++y)
	{
		for (u32 x = 0; x < horizontalVertices; ++x)
		{
			vertices.Add(SVertex2D(glm::vec3(x * sm_TileWidth, y * sm_TileHeight, 0.0f), glm::vec2(x, y)));
		}
	}

	// generate indices
	for (u32 y = 0; y < m_ChunkHeight; ++y)
	{
		for (u32 x = 0; x < m_ChunkWidth; ++x)
		{
			u32 idx = x + (y * verticalVertices);
			indices.Add(idx);
			indices.Add(idx + verticalVertices);
			indices.Add(idx + 1);			

			indices.Add(idx + 1);
			indices.Add(idx + 1 + horizontalVertices);
			indices.Add(idx + horizontalVertices);
		}
	}
	*/

	const float CellUVWidth = 1.f / m_ChunkWidth;
	const float CellUVHeight = 1.f / m_ChunkHeight;
	const float CellUVHalfWidth = CellUVWidth / 2.f;
	const float CellUVHalfHeight = CellUVHeight / 2.f;
	for (u32 y = 0; y < m_ChunkHeight; ++y)
	{
		for (u32 x = 0; x < m_ChunkWidth; ++x)
		{
			// create all the vertices and indices
			u32 FirstIndex = u32(vertices.size());

			glm::vec2 UV = glm::vec2(float(x) / m_ChunkWidth + CellUVHalfWidth, float(y) / m_ChunkHeight + CellUVHalfHeight);

			// Add vertices of quad
				// Top left (0, 1)
			vertices.Add(SVertex2D(
				glm::vec2(x * sm_TileWidth, y * sm_TileHeight + sm_TileHeight), 
				glm::vec2(0.0001f, 0.9999f), UV));
				// Bot left (0, 0)
			vertices.Add(SVertex2D(
				glm::vec2(x * sm_TileWidth, y * sm_TileHeight), 
				glm::vec2(0.0001f, 0.0001f), UV));
				// Bot right (1, 0)
			vertices.Add(SVertex2D(
				glm::vec2(x * sm_TileWidth + sm_TileWidth, y * sm_TileHeight), 
				glm::vec2(0.9999f, 0.0001f), UV));
				// Top right (1, 1)
			vertices.Add(SVertex2D(
				glm::vec2(x * sm_TileWidth + sm_TileWidth, y * sm_TileHeight + sm_TileHeight), 
				glm::vec2(0.9999f, 0.9999f), UV));
			
			// Add indices of quad
				// first tri: tl, bl, br
			indices.Add(FirstIndex);
			indices.Add(FirstIndex + 1);
			indices.Add(FirstIndex + 2);
				// second tri: br, tr, tl
			indices.Add(FirstIndex + 2);
			indices.Add(FirstIndex + 3);
			indices.Add(FirstIndex);
		}
	}

	m_NumIndices = u32(indices.size());

	// generate vao
	u32 VBO, EBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex2D), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(u32), indices.data(), GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (void*)offsetof(SVertex2D, m_UV));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (void*)offsetof(SVertex2D, m_TilemapUV));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	LoadLandscapeProgram();
}

void CLandscapeChunk::SetTileAtlas(Rendering::CTexture a_TileAtlas)
{
	m_TileAtlas = a_TileAtlas;
}

void CLandscapeChunk::SetTilemap(Rendering::CTexture a_Texture)
{
	m_Tilemap = a_Texture;
}

void CLandscapeChunk::Draw(glm::mat4 a_MVP)
{
	// Use the landscape shader
	sm_Landscape2DProgram.Use();

	sm_Landscape2DProgram.SetFloatValue("ChunkWidthUnits", float(m_ChunkWidth * sm_TileWidth));
	sm_Landscape2DProgram.SetFloatValue("ChunkHeightUnits", float(m_ChunkHeight * sm_TileHeight));
	sm_Landscape2DProgram.SetFloatValue("HorizontalTiles", float(m_ChunkWidth));
	sm_Landscape2DProgram.SetFloatValue("VerticalTiles", float(m_ChunkHeight));

	sm_Landscape2DProgram.SetVec("TilesetResolution", glm::vec2(m_TileAtlas.GetWidth(), m_TileAtlas.GetHeight()));
	sm_Landscape2DProgram.SetVec("TileResolution", glm::vec2(56.f));
	sm_Landscape2DProgram.SetVec("TilePadding", glm::vec2(4.f));

	sm_Landscape2DProgram.SetIntValue("Tileset", 0);
	sm_Landscape2DProgram.SetIntValue("Tilemap", 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_TileAtlas.GetTextureID());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_Tilemap.GetTextureID());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0); // Reset back to first texture slot
}

}