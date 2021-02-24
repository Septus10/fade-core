#include <FrameBuffer.hpp>

#include <ApiInclude.hpp>

namespace Fade { inline namespace RHI {

//===================================================================
// CBuffer
CBuffer::CBuffer()
	: m_Format(EAttachmentBufferFormat::U32)
	, m_Type(EAttachmentType::Color)
{ }
//===================================================================

//===================================================================
// IFrameBuffer
FADE_RHI_API IFrameBuffer* IFrameBuffer::Create()
{
	return new CFrameBuffer();
}
//===================================================================

CFrameBuffer::CFrameBuffer()
	: m_FBO(0)
{ }

bool CFrameBuffer::Create(u32 a_Width, u32 a_Height)
{
	// Generate frame buffer and bind it
	glGenFramebuffers(1, &m_FBO);
	Bind();

	// Add initial attachment
	if (!AddAttachment(EAttachmentType::Color))
	{
		// LOG ERROR
		return false;
	}

	// check if it's complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// LOG ERROR
		return false;
	}

	return true;
}

void CFrameBuffer::Destroy()
{
	glDeleteFramebuffers(1, &m_FBO);
}

bool CFrameBuffer::Resize(u32 a_NewWidth, u32 a_NewHeight)
{
	return true;
}

bool CFrameBuffer::AddAttachment(EAttachmentType a_Type)
{
	return true;
}

void CFrameBuffer::Bind()
{
	if (m_FBO != 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	}
}

void CFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

class CTextureBuffer : public CBuffer
{
public:
	virtual void Create(EAttachmentBufferFormat a_Format, EAttachmentType a_Type, u32 a_Width, u32 a_Height) override
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		i32 Format			= GL_RGB;
		i32 InternalFormat	= GL_RGB;
		i32 Type			= GL_UNSIGNED_BYTE;
		switch (a_Type)
		{
		case EAttachmentType::Color:
			switch (a_Format)
			{
			case EAttachmentBufferFormat::U8:
				Format = GL_R;
				break;
			case EAttachmentBufferFormat::U16:
				Format = GL_RG;
				break;
			case EAttachmentBufferFormat::U24:
				Format = GL_RGB;
				break;
			case EAttachmentBufferFormat::U32:
				Format = GL_RGBA;
				break;
			}
			break;
		case EAttachmentType::Depth:
			Format = InternalFormat = GL_DEPTH_COMPONENT;
			break;
		case EAttachmentType::Stencil:
			Format = InternalFormat = GL_STENCIL_INDEX;
			break;
		case EAttachmentType::DepthStencil:
			InternalFormat = GL_DEPTH24_STENCIL8;
			Format = GL_DEPTH_STENCIL;
			Type = GL_UNSIGNED_INT_24_8;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, a_Width, a_Height, 0, Format, Type, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Making an assumption that when this function is called, the framebuffer is bound
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);
	}

private:
	u32 m_Texture;
};

class CRenderBuffer : public CBuffer
{
public:
	virtual void Create(EAttachmentBufferFormat a_Format, EAttachmentType a_Type, u32 a_Width, u32 a_Height)
	{

	}

	virtual void Resize(u32 a_Width, u32 a_Height) override
	{

	}
};

bool CFrameBuffer::CreateTextureAttachment(EAttachmentType a_Type)
{
	return true;
}

bool CFrameBuffer::CreateRenderBufferAttachment(EAttachmentType a_Type)
{
	return false;
}

} }