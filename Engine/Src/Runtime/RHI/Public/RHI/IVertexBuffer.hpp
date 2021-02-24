#pragma once

#include <RHI/Types.hpp>

#include <Core/Pimpl.hpp>
#include <Core/Containers/Array.hpp>
#include <memory>

namespace Fade { inline namespace RHI {

enum class EBufferAccessFrequency : u8
{
	
};

enum class EBufferAccessType : u8
{
	CPUW_GPUR,	// Modified by CPU, read by GPU
	CPUR_GPUW,	// Modified by GPU, read by CPU
	GPURW		// Modified and read by GPU
};

/**
 * Vertex buffer interface
 */
class IVertexBuffer
{
public:
	/**
	 * Sets the data of the vertex array
	 */
	virtual void SetData(SVertex* a_VertexArray, usize a_NumVertices) = 0;

	/**
	 * Resizes data buffer to requested size
	 */
	virtual void Resize(usize a_NumVertices) = 0;

	/**
	 * Draw the vertex buffer
	 */
	virtual void Draw() = 0;
};

FADE_RHI_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer();
} }