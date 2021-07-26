#include <Core/Reflection/Reflectable.hpp>

namespace Fade {

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

std::size_t SHA256::Hash(const std::string& a_String)
{
	const char* s = a_String.c_str();
	std::size_t h = FIRSTH;
	while (*s != '\0')
	{
		h = (h * A) ^ (std::size_t(s[0]) * B);
		s++;
	}
	return h; // or return h % C;
}

CReflectable::CReflectable(const char* a_Name)
	: m_Name(std::string(a_Name))
{
	m_UniqueID = Fade::SHA256::Hash(a_Name);
}

}