#pragma once

#ifdef FADE_EXPORT
#define FADE_CORE_API __declspec(dllexport)
#else
#define FADE_CORE_API __declspec(dllimport)
#endif