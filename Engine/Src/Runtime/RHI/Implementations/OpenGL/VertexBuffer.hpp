#pragma once

#include <RHI/IVertexBuffer.hpp>

#include <Core/Definitions.hpp>

namespace Fade { inline namespace RHI {

/**
 * Vertex buffer class
 *
 * Contains data for both vertices and indices when applicable.
 */

class FADE_RHI_API CVertexBuffer : public IVertexBuffer
{
public:
	/**
	 * Construct an empty vertex buffer
	 */
	CVertexBuffer();

	/**
	 * Construct a vertex buffer from a vertex array
	 */
	CVertexBuffer(SVertex* a_VertexArray, usize a_NumVertices);

	/**
	 * Construct a vertex buffer from a dynamic array
	 */
	CVertexBuffer(TArray<SVertex>& a_VertexArray);

	/**
	 * Construct a vertex buffer with a designated size
	 */
	CVertexBuffer(usize a_NumVertices);

	/**
	 *
	 */
	~CVertexBuffer()
	{}

	/**
	 * Sets the data of the vertex array
	 */
	virtual void SetData(SVertex* a_VertexArray, usize a_NumVertices) override;

	/**
	 * Resizes data buffer to requested size
	 */
	virtual void Resize(usize a_NumVertices) override;

	/**
	 *
	 */
	virtual void Draw() override;

private:
	u32 VAO;
	u32 VBO, EBO;

	void InitializeBuffers();

	usize m_Size;
	usize m_AllocatedSize;
};


} }