#pragma once

#include "../Resource.hpp"

#include <string>

namespace Fade {

class IResourceImporter
{
public:
	virtual CResource* ImportResource(std::string a_FilePath) = 0;

private:


};

}