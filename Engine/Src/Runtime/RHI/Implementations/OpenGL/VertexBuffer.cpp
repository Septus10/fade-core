#include <VertexBuffer.hpp>

#include <Core/Log.hpp>

#include "ApiInclude.hpp"

namespace Fade { inline namespace RHI {

CVertexBuffer::CVertexBuffer()
	: VAO(0)
	, VBO(0)
	, EBO(0)
	, m_Size(0)
	, m_AllocatedSize(0)
{
}

CVertexBuffer::CVertexBuffer(SVertex* a_VertexArray, usize a_NumVertices)
	: VAO(0)
	, VBO(0)
	, EBO(0)
	, m_Size(0)
	, m_AllocatedSize(0)
{
	Resize(a_NumVertices);
	SetData(a_VertexArray, a_NumVertices);
}

CVertexBuffer::CVertexBuffer(TArray<SVertex>& a_VertexArray)
	: VAO(0)
	, VBO(0)
	, EBO(0)
	, m_Size(0)
	, m_AllocatedSize(0)
{
	Resize(a_VertexArray.Num());
	SetData(a_VertexArray.data(), a_VertexArray.Num());
}

CVertexBuffer::CVertexBuffer(usize a_NumVertices)
	: VAO(0)
	, VBO(0)
	, EBO(0)
	, m_Size(0)
	, m_AllocatedSize(0)
{
	Resize(a_NumVertices);
}

void CVertexBuffer::SetData(SVertex* a_VertexArray, usize a_NumVertices)
{
	if (m_Size != a_NumVertices)
	{
		m_Size = a_NumVertices;
		Resize(a_NumVertices);
	}
	
	void* Data = glMapNamedBuffer(VBO, GL_WRITE_ONLY);
	if (Data == nullptr)
	{
		FADE_LOG(Error, VertexBuffer, "Unable to map VBO");
	}

	memcpy(Data, a_VertexArray, sizeof(SVertex) * a_NumVertices);

	if (!glUnmapNamedBuffer(VBO))
	{
		// Log error, we couldn't map data to buffer
		FADE_LOG(Error, VertexBuffer, "Unable to unmap VBO");
	}
}

void CVertexBuffer::Resize(usize a_NumVertices)
{
	if (m_AllocatedSize == 0)
	{
		InitializeBuffers();
	}

	// Bind VAO & VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SVertex) * a_NumVertices, nullptr, GL_DYNAMIC_DRAW);

	// Vertex attrib pointers, location, uv and color
		// Location
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
	glEnableVertexAttribArray(0);
		// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, U));
	glEnableVertexAttribArray(1);
		// Color
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(SVertex), (void*)offsetof(SVertex, RGBA));
	glEnableVertexAttribArray(2);
		// Texture ID
	glVertexAttribPointer(3, 1, GL_INT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, TexSlot));
	glEnableVertexAttribArray(3);

	// TODO Error handling

	// Set allocated size to new size
	m_AllocatedSize = a_NumVertices;
}

void CVertexBuffer::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_Size);
}

void CVertexBuffer::InitializeBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

FADE_RHI_API std::shared_ptr<IVertexBuffer> CreateVertexBuffer()
{
	return std::make_shared<CVertexBuffer>();
}

} }

