#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Math/Math.hpp>

namespace Fade {

struct SColor
{
	FADE_CORE_API static SColor Red;
	FADE_CORE_API static SColor Green;
	FADE_CORE_API static SColor Blue;
	FADE_CORE_API static SColor Yellow;
	FADE_CORE_API static SColor Cyan;
	FADE_CORE_API static SColor Magenta;
	FADE_CORE_API static SColor Black;
	FADE_CORE_API static SColor White;

	SColor(float a_R = 0.f, float a_G = 0.f, float a_B = 0.f, float a_A = 1.f)
		: m_A(a_A)
		, m_R(a_R)
		, m_G(a_G)
		, m_B(a_B)
	{ }

	float m_A, m_R, m_G, m_B;
};

}