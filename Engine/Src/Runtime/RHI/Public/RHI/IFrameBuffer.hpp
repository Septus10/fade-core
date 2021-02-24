#pragma once

#include <RHI/RHIApi.hpp>

namespace Fade { inline namespace RHI {

enum class EAttachmentBufferType : u8
{
	Internal,	// Also known as render buffers, fast for drawing and GPU usage, slow for querying pixels on CPU
	Texture		// Slow for drawing and GPU usage, fast for querying pixels on CPU
};

enum class EAttachmentBufferFormat : u8
{
	U8,
	U16,
	U24,
	U32
};

enum class EAttachmentType : u8
{
	Color,
	Depth,
	Stencil,
	DepthStencil,
	SIZE
};


class IFrameBuffer
{
public:
	static FADE_RHI_API IFrameBuffer* Create();

	virtual bool Create(u32 a_Width, u32 a_Height) = 0;
	virtual void Destroy() = 0;

	virtual bool Resize(u32 a_NewWidth, u32 a_NewHeight) = 0;

	virtual bool AddAttachment(EAttachmentType a_Type) = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
};

} }