#pragma once

#include <fstream>

namespace fade
{
	namespace fstl
	{
		using fstream = std::fstream;

		using istream = std::istream;
		using ifstream = std::ifstream;

		using ostream = std::ostream;
		using ofstream = std::ofstream;
	}
}