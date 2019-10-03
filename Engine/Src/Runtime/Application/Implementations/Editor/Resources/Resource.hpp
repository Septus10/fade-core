#pragma once

#include <string>

namespace Fade { 

class CResource
{
public:
	const std::string& GetName()
	{
		return m_Name;
	}

private:
	std::string m_Name;
};

}