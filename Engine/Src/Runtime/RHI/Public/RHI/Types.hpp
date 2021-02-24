#pragma once

#include <Core/Definitions.hpp>

#include <RHI/RHIApi.hpp>

namespace Fade { inline namespace RHI {

	//struct TextureFormat {
	//	static const u32 FADE_RHI_API RGBA;
	//	static const u32 FADE_RHI_API BGRA;
	//	static const u32 FADE_RHI_API RGB;
	//	static const u32 FADE_RHI_API BGR;
	//};
	//
	//struct DataType {
	//	static const u32 FADE_RHI_API BYTE;
	//	static const u32 FADE_RHI_API UBYTE;
	//	static const u32 FADE_RHI_API SHORT;
	//	static const u32 FADE_RHI_API USHORT;
	//	static const u32 FADE_RHI_API INT;
	//	static const u32 FADE_RHI_API UINT;
	//	static const u32 FADE_RHI_API FLOAT;
	//	static const u32 FADE_RHI_API DOUBLE;
	//};

struct FADE_RHI_API SVertex
{
	/**
	 * X, Y and Z coordinates
	 */
	float X, Y, Z;

	/**
	 * UV coordinates
	 */
	float U, V;

	/**
	 * Color value in RGBA format
	 */
	u32 RGBA;

	/**
	 * Texture slot index
	 */
	i32 TexSlot;
};

} }