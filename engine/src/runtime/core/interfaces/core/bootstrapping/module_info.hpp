#pragma once
#include <definitions.hpp>
#include <core/hash.hpp>

#define FADE_MODULE_DEFINE(module_name)												\
	class module_##module_name														\
	{																				\
	public:																			\
		static constexpr const fade::c8* const name = (#module_name);				\
		static constexpr const fade::u64 name_hash = FADE_HASH_FNV1A(#module_name);	\
	};