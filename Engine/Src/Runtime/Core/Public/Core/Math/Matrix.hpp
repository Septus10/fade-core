#pragma once

#include <Core/Definitions.hpp>
#include <Core/CoreApi.hpp>

namespace Fade { namespace Math {

template <int Rows, int Cols>
struct Matrix
{
	//=========================================================================
	// Members
	//=========================================================================
	float m_Data[Cols][Rows];

	//=========================================================================
	// Statics
	//=========================================================================
	
};

using Mat4 = Matrix<4, 4>;
using Mat3 = Matrix<3, 3>;

} }