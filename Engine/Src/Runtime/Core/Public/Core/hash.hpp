#pragma once

#include <Core/Definitions.hpp>
#include <Core/CoreApi.hpp>
#include <array>
#include <vector>
#include <string>

#define FADE_HASH_FNV1A(content) __pragma(warning(push)) __pragma(warning(disable: 4307)) ::Fade::CHash::fnv1a(content) __pragma(warning(pop))

namespace Fade
{

class CHash
{
private:
    static constexpr u64 fnv1a_rec(const c8* str, u64 hash)
    {
        return *str == '\0'
            ? hash
            : fnv1a_rec(str + 1, (hash ^ static_cast<u64>(*str)) * 1099511628211ULL);
    }

public:
    // Computes the FNV-1a hash
    static constexpr u64 fnv1a(const c8* str)
    {
        return str == nullptr || *str == '\0'
            ? 0
            : fnv1a_rec(str, 14695981039346656037ULL);
    }

    static FADE_CORE_API std::string sha256(std::vector<u8>& data);

    static FADE_CORE_API std::string sha256(std::string& data);
};

}
