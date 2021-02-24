#pragma once

#include <Core/Definitions.hpp>

#include <Core/Containers/Array.hpp>

#include <RHI/IFrameBuffer.hpp>

namespace Fade { inline namespace RHI {

class CBuffer
{
public:
	CBuffer();
	virtual void Create(EAttachmentBufferFormat a_Format, EAttachmentType a_Type, u32 a_Width, u32 a_Height) = 0;
	virtual void Attach() = 0;
	virtual void Resize(u32 a_Width, u32 a_Height) = 0;

private:
	EAttachmentBufferFormat		m_Format;
	EAttachmentType				m_Type;
};


class CFrameBuffer : public IFrameBuffer
{
public:
	CFrameBuffer();

	virtual bool Create(u32 a_Width, u32 a_Height) override;
	virtual void Destroy() override;

	virtual bool Resize(u32 a_NewWidth, u32 a_NewHeight) override;

	virtual bool AddAttachment(EAttachmentType a_Type) override;

	virtual void Bind() override;
	virtual void Unbind() override;

private:
	bool CreateTextureAttachment(EAttachmentType a_Type);
	bool CreateRenderBufferAttachment(EAttachmentType a_Type);

	u32 m_FBO;
	u32 m_Width, m_Height;

	struct SBufferStorage
	{
		TArray<CBuffer*> m_Buffers;
	};
	SBufferStorage m_BufferStorage[(usize)EAttachmentType::SIZE];
};

} }