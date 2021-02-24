#include <RHI/RHI.hpp>

#include <ApiInclude.hpp>

namespace Fade { inline namespace RHI {

bool g_IsRHIInitialized = false;

namespace TextureFormat {
u32 RGBA	= GL_RGBA;
u32 BGRA	= GL_BGRA;
u32 RGB		= GL_RGB;
u32 BGR		= GL_BGR;
}

namespace DataType {
u32 BYTE	= GL_BYTE;
u32 UBYTE	= GL_UNSIGNED_BYTE;
u32 SHORT	= GL_SHORT;
u32 USHORT	= GL_UNSIGNED_SHORT;
u32 INT		= GL_INT;
u32 UINT	= GL_UNSIGNED_INT;
u32 FLOAT	= GL_FLOAT;
u32 DOUBLE	= GL_DOUBLE;
}

i32 g_MaxTextureMipCount;
i32 g_MaxTextureCount;

} }