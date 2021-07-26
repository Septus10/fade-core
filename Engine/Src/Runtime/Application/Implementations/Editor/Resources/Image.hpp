#pragma once

#include <Core/Definitions.hpp>
#include "Resource.hpp"

namespace Fade {

struct SImageData
{
	usize m_Width;
	usize m_Height;
	u8* m_Data;
};

class CImage : public CResource
{
public:
	

private:
	SImageData m_ImageData;
};

}