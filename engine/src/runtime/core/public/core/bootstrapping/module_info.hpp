#pragma once
#include <core/definitions.hpp>
#include <core/hash.hpp>

#define FADE_MODULE_DEFINE(module_name)												\
	class CModule##module_name														\
	{																				\
	public:																			\
		static constexpr const Fade::c8* const name = (#module_name);				\
		static constexpr const Fade::u64 name_hash = FADE_HASH_FNV1A(#module_name);	\
	};